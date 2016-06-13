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
} infoTomadas;

class Gerente;

class Mensageiro{

public:

	void enviarViaNIC(infoTomadas msg){
		NIC nic;
		nic.send(NIC::BROADCAST, (NIC::Protocol)1, &msg,sizeof(msg));
		Alarm::delay(100000);
	}
	static int receberViaNIC();	

	Mensageiro(Gerente * gnt);

private:
	static Gerente *gerente;

};

Gerente * Mensageiro::gerente;

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
	}
protected:
	int prioridade;
	double consumoMaximo;
	double consumoMinimo;
	bool PowerSaving;
	int address;
};

//--------------------------Gerente-----------------------------------------

class Gerente{

public:

void enviarAtualizacao(){
	infoTomadas msg;
	msg.previsao = tomada.getConsumo();
	msg.address = tomada.getAddress();
	msg.prioridade = tomada.getPrioridade();
	msg.tipo = 0;
	msngr.enviarViaNIC(msg);
}

void receberAtualizacao(infoTomadas msg){
	cout<<"###########################\n";
	cout<<"MensagemRecebida!\n";
	cout<<"Sua previsao eh: " << msg.previsao << "\n";
	cout<<"Seu endereco eh: " << msg.address << "\n";
	cout<<"Sua prioridade eh: " << msg.prioridade << "\n";
	cout<<"Seu tipo eh: " << msg.tipo << "\n";
}

Gerente(TomadaComSensor t) :tomada(t), msngr(this){
	numTomadasExternas = 0;
	diaAtual = 0;
	horaAtual = 0;
	previsaoPropria = 0;
	previsaoGeral = 0;
}

private:
infoTomadas tomadasExternas[0];
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
				gerente->receberAtualizacao(meg);
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

	g.enviarAtualizacao();

}
