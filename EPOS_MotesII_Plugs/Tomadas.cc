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
	bool pule;
	bool atualizado;
} infoTomadas;

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
	int getPrioridade(){
		return prioridade;
	}
	void setPrioridade(int prio){
		prioridade = prio;
	}
	TomadaComSensor(){
		PowerSaving = false;
		prioridade = 15;
		consumoMaximo = 1220;
		consumoMinimo = 255000;
		address = 0x2;
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
			if(msg.tipo == 1){
				tomadasExternas[i].pule = true;
			} else {
				tomadasExternas[i].pule = false;
			}
			imprimirArray();
			return;
		}
	}
	adicionarTomada(msg);
	imprimirArray();
}

void fazerPrevisaoPropria(){
	previsaoPropria = Previsor::preverProprio(mediaPorDia,diaAtual);
}

void fazerPrevisaoTotal(){
	previsaoGeral = Previsor::preverTotal(tomadasExternas,numTomadasExternas,previsaoPropria);
}

void adicionarTomada(infoTomadas t){
	if(numTomadasExternas <= 20){
	tomadasExternas[numTomadasExternas].atualizado = true;
	tomadasExternas[numTomadasExternas].address = t.address;
	tomadasExternas[numTomadasExternas].previsao = t.previsao;
	tomadasExternas[numTomadasExternas].prioridade = t.prioridade;
	tomadasExternas[numTomadasExternas].pule = false;

	numTomadasExternas++; 
	}
}

void imprimirArray(){
	for(int i = 0; i < numTomadasExternas; i++){
		cout<<"##################\n";
		cout<<"tomada " << i << "\n";
		cout<<"endereco: " << tomadasExternas[i].address << "\n";
		cout<<"previsao: " << tomadasExternas[i].previsao<< "\n";
		cout<<"prioridade: " << tomadasExternas[i].prioridade << "\n";
		cout<<"atualizado: " << tomadasExternas[i].atualizado << "\n";
		cout<<"pule: " << tomadasExternas[i].pule << "\n";
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
