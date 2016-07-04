#include <nic.h>
#include <utility/ostream.h>
#include <utility/random.h>
#include <alarm.h>
#include <clock.h>

__USING_SYS

OStream cout;

const unsigned int GPIO_BASE =0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;

typedef struct {
	int previsao;
	int address;
	int prioridade;
	int tipo;
	bool atualizado;
} infoTomadas;

enum acao {ATUALIZACAO = 0, NONE = 1, DESLIGAR = 2, DIM = 3, LIGAR = 4};

//Retirei o pule pois mesmo que a tomada esteja desligada ela ainda assim mandará atualizações que basicamente dirao que ela esta desligada.
//Afinal se ela desligar por comleto quando que ela vai ligar se parar até de receber mensagens? E o consumo dela até o momento que ela desligou?
//Nao entra em consideracao?

class Gerente;

class Mensageiro{

public:

	void enviarViaNIC(infoTomadas msg){
		NIC nic;
		nic.send(NIC::BROADCAST, (NIC::Protocol)1, &msg,sizeof(msg));
		Alarm::delay(10000);
	}
	static int receberViaNIC();	

	Mensageiro(Gerente * gnt);

private:
	static Gerente *gerente;

};

Gerente * Mensageiro::gerente;

//----------------------------------Previsor---------------------------------------

class Previsor{

public:

static double preverDia(double mediaPorHora[24],int horaAtual){
	double soma = 0;
	for(int i = 0; i < horaAtual+1;i++){
		soma += mediaPorHora[i];
	}
	double media = soma / horaAtual;
	soma += media * (24 - horaAtual);
	return soma;
}

static double preverProprio(double mediaPorDia[30], int diaAtual){
	double soma = 0;
	for(int i = 0; i < diaAtual; i++){
		soma += mediaPorDia[i];
	}
	 
	double media = soma/diaAtual;
	
	soma += media * (30 - diaAtual);
	
	return soma;
}

static double preverTotal(infoTomadas *tomadas, int sizeTomadas, double previsaoPropria){
	double previsaoTotal = 0;
	for(int i = 0; i < sizeTomadas;i++){
		previsaoTotal += tomadas->previsao;
	}
	previsaoTotal += previsaoPropria;
	return previsaoTotal;
}

};


//--------------------------Leds---------------------------------------------------

class ledTest{

public:
static int turn_led(int led, bool on){
	
	int          bit     = led % 32;
	unsigned int regData = GPIO_DATA_SET0 + ( ( led >> 5 ) << 2 );
    	unsigned int regPad  = GPIO_PAD_DIR0  + ( ( led >> 5 ) << 2 );
    	unsigned int value   = CPU::in32( regPad );
	
	if(!on){
		//turn off
		value &= ~( 1 << bit );		

	} else {  
		//turn on
		value |= ( 1 << bit );
	}

	CPU::out32(regPad,value);
	CPU::out32(regData, ( 1 << bit ) );

	return 0;
}
};

//----------------------Tomadas------------------------------------------------------

class Tomada{

public:
	bool estaLigado(){
		return ligado;
	}

	void ligar(){
		ligado = true;
		ledTest::turn_led(23,true);
	}
	void desligar(){
		ligado = false;
		ledTest::turn_led(23,false);
	}
	Tomada(){
		ligado = false;
		ledTest::turn_led(23,false);
	}

	int getTipo(){
		return -1;
	}

protected:
	bool ligado;
};


class TomadaComSensor : public Tomada{
public:
	double getConsumo(){
		//double consumo = sensor->sample();
		double consumo = (double)((Pseudo_Random::random() * Pseudo_Random::random()) % (int)consumoMaximo);
		if(consumo < consumoMinimo && consumo != 0){
			consumoMinimo = consumo;
		}
		if(consumo > consumoMaximo){
			consumoMaximo = consumo;
		}
		if(ligado){
			return consumo;
		} else {
			return -1;
		}
	}
	int getTipo(){
		return 0;
	}
	int getAddress(){
		return address;
	}
	double getConsumoMaximo(){
		return consumoMaximo;
	}
	double getConsumoMinimo(){
		return consumoMinimo;
	}
	double getMediaDeConsumo(){
		return (consumoMaximo + consumoMinimo)/2;
	}
	bool getPowerSavingMode(){
		return PowerSaving;
	}
	void setPowerSavingMode(bool on){
		PowerSaving = on;
	}
	void setPowerSavingLimit(int limit = 0){
		limite = limit;
	}
	int getPowerSavingLimit(){
		return limite;
	}
	int getPrioridade(){
		return prioridade;
	}
	void setPrioridade(int prio = 0){
		prioridade = prio;
	}
	TomadaComSensor(){
		PowerSaving = false;
		prioridade = 0;
		consumoMaximo = 1220;
		consumoMinimo = 255000;
		address = 0x1;
		limite = 0;
	}
protected:
	int prioridade;
	double consumoMaximo;
	double consumoMinimo;
	bool PowerSaving;
	int address;
	double limite;
};


class TomadaDimer : public Tomada{

public:

void dim(int porcentagem){
	if(porcentagem < 100 && porcentagem > 0){
		dimPorcent = porcentagem;
	} else {
		cout<< "porcentagem invalida\n";
	}
}

void dimLed(){
}

TomadaDimer() {//: dimerThread(&dimLed){
	dimPorcent = 0;
}

private:
	int dimPorcent;
	//Thread dimerThread;

};

class TomadaTop : public TomadaComSensor , public TomadaDimer{

int getTipo(){
	return 1;
}

TomadaTop(){}

};

//--------------------------Gerente-----------------------------------------

class Gerente{

public:

void enviarMensagem(int tipo){
	infoTomadas msg;
	fazerPrevisaoPropria();
	msg.previsao = previsaoPropria;
	msg.address = tomada.getAddress();
	msg.prioridade = tomada.getPrioridade();
	msg.tipo = tipo;
	msngr.enviarViaNIC(msg);
}

void receberMensagem(infoTomadas msg){
	for(int i = 0; i < numTomadasExternas; i++){
		if(msg.address == tomadasExternas[i].address){
			tomadasExternas[i].atualizado = true;
			tomadasExternas[i].previsao = msg.previsao;
			tomadasExternas[i].prioridade = msg.prioridade;
			tomadasExternas[i].tipo = msg.tipo;
			imprimirArray(); //debug purposes
			return;
		}
	}
	adicionarTomada(msg);
	imprimirArray(); //debug purposes
}

void fazerPrevisaoPropria(){
	previsaoPropria = Previsor::preverProprio(mediaPorDia,diaAtual);
}

void fazerPrevisaoTotal(){
	previsaoGeral = Previsor::preverTotal(tomadasExternas,numTomadasExternas,previsaoPropria);
}

void adicionarTomada(infoTomadas t){
	if(numTomadasExternas <= 20){ // deve ser depois alterado para algo como uma lista encadeada, de tamanho variavel.
		tomadasExternas[numTomadasExternas].atualizado = true;
		tomadasExternas[numTomadasExternas].address = t.address;
		tomadasExternas[numTomadasExternas].previsao = t.previsao;
		tomadasExternas[numTomadasExternas].prioridade = t.prioridade;
		tomadasExternas[numTomadasExternas].tipo = t.tipo;

		numTomadasExternas++; 
	}
}

void imprimirArray(){  //function for debug purposes
	for(int i = 0; i < numTomadasExternas; i++){
		cout<<"##################\n";
		cout<<"tomada " << i << "\n";
		cout<<"endereco: " << tomadasExternas[i].address << "\n";
		cout<<"previsao: " << tomadasExternas[i].previsao<< "\n";
		cout<<"prioridade: " << tomadasExternas[i].prioridade << "\n";
		cout<<"atualizado: " << tomadasExternas[i].atualizado << "\n";
	}
}

void tomadaInteligente(){
	
	bool myTurn = true;

	if(previsaoGeral > tomada.getPowerSavingLimit()){
		for(int i = 0; i < numTomadasExternas;i++){
			if(tomadasExternas[i].prioridade < tomada.getPrioridade()){
				if(tomadasExternas[i].tipo == NONE){
					myTurn = false;
					break;
				} 
			}
		}
	} else {
		if(tomada.estaLigado()){
			enviarMensagem(NONE);
		} else {
			if(tomada.getConsumoMaximo() + previsaoGeral > tomada.getPowerSavingLimit()){
				enviarMensagem(DESLIGAR);
			} else {
				enviarMensagem(LIGAR);
			}
		}
	}

	if(myTurn){
		if(tomada.getConsumo() < tomada.getMediaDeConsumo()){
			// tipo 0 = sem dimer
			if(tomada.getTipo() == 0){
				tomada.desligar();
				enviarMensagem(DESLIGAR);
				
			} else {
				//tomada.dim(50);
				enviarMensagem(DIM);
			}
		}
			
	}
}


Gerente(TomadaComSensor t) :tomada(t), msngr(this){
	numTomadasExternas = 0;
	diaAtual = 0;
	horaAtual = 0;
	previsaoPropria = 0;
	previsaoGeral = 0;
}

private:
infoTomadas tomadasExternas[20];
int numTomadasExternas;
double mediaPorHora[24];
double mediaPorDia[30];
int diaAtual;
int horaAtual;
double previsaoPropria;
double previsaoGeral;


TomadaComSensor tomada;
Mensageiro msngr;

};

//------------------------------------Mensageiro-----------------------------------

int Mensageiro::receberViaNIC(){
		NIC nic;		
		NIC::Address src;
		unsigned char prot;
		infoTomadas meg;
		while(1){
			while(!nic.receive(&src, &prot,&meg,sizeof(meg)) >0);
			if(meg.address != 0){
				gerente->receberMensagem(meg);
			}
			meg.address = 0;
		}
		return 0;
	}	

Mensageiro::Mensageiro(Gerente * gnt){
		gerente = gnt;
		Thread *thread;
		thread = new Thread( &receberViaNIC );
}



//------------------------------------Main-------------------------------------------
int main(){
	
	TomadaComSensor t;
	Gerente g(t);

	t.setPrioridade(10);
	cout<< "primeiro consumo: " << (int)t.getConsumo() << "\n";
	t.ligar();
	int i = 10;
	while (i--){
		cout<< "consumo: " << (int)t.getConsumo() << "\n";
	}
	cout<< t.getPrioridade() << "\n";
	cout<< t.estaLigado() << "\n";

	g.enviarMensagem(0);


}
