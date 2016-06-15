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

/**
* Classe responsável por gerenciar a tomada e manter atualizada.
*/
class Gerente{

/**
* Metodos e atributos publicos da classe Gerente
*/
public:

/**
* Metodo sem retorno para enviar atualizacoes sobre a tomada.
* @param int tipo
*/
void enviarMensagem(int tipo){
	/**
	* Struct utilizada para armazenar as informacoes sobre a tomada
	*/
	infoTomadas msg;
	/**
	* Metodo para fazer a previsao de consumo da propria tomada
	*/
	fazerPrevisaoPropria();
	/**
	* Atribui a previsao calculada para a tomada como valor da previsao da mensagem.
	*/
	msg.previsao = previsaoPropria;
	/**
	* Atribui o endereco da tomada ao endereco da struct msg
	*/
	msg.address = tomada.getAddress();
	/**
	* Atribui a prioridade da tomada a prioridade da struct msg
	*/
	msg.prioridade = tomada.getPrioridade();
	/**
	* Atribui o tipo da tomada para o tipo da struct msg
	*/
	msg.tipo = tipo;
	/**
	* chama o metodo para enviar a mensagem com as informacoes da tomada
	*/
	msngr.enviarViaNIC(msg);
}

/**
* Metodo sem retorno para receber atualizacoes sobre as demais tomadas da rede.
* @param inforTomadas msg
*/

void receberMensagem(infoTomadas msg){
	/**
	* laco utilizado para percorrer o array de tomadas externas.
	*/
	for(int i = 0; i < numTomadasExternas; i++){
		/**
		* verifica se o endereco da tomada que enviou a mensagem eh o mesmo
		* da tomada que o for esta passando.
		*/
		if(msg.address == tomadasExternas[i].address){
			/**
			* Se for igual atualiza o parametro da tomada externa para indicar
			* que esta atualizada
			*/
			tomadasExternas[i].atualizado = true;
			/**
			* atualiza a previsao de consumo da tomada externa com o valor
			* transmitido pela mensagem
			*/
			tomadasExternas[i].previsao = msg.previsao;
			/**
			* configura a prioridade da tomada para a prioridade passada na mensagem
			*/
			tomadasExternas[i].prioridade = msg.prioridade;
			/**
			* verifica se o tipo de mensagem eh do tipo 1, se for verdade configura para pular.
			* Em caso contrario configura para não pular.
			*/
			if(msg.tipo == 1){
				tomadasExternas[i].pule = true;
			} else {
				tomadasExternas[i].pule = false;
			}
			/**
			* chama o metodo para imprimir as informacoes atualizadas do array de tomadas externas.
			*/
			imprimirArray();
			/**
			* finaliza o metodo chamando um return sem retorno.
			*/
			return;
		}
	}
	/**
	* caso seja verificado todo o array de tomadas externas e não for encontrada a tomada
	* que enviou a mensagem, esta eh adicionada no array de tomadas externas.
	*/
	adicionarTomada(msg);
	/**
		* chama o metodo para imprimir as informacoes atualizadas do array de tomadas externas.
	*/
	imprimirArray();
}

/**
* Metodo sem retorno que faz o calculo da previsao de consumo.
*/

void fazerPrevisaoPropria(){
	/**
	* atribuicao do retorno da chamada Previsor::preverProprio(mediaPorDia, diaAtual)
	* para a variavel previsaoPropria
	*/
	previsaoPropria = Previsor::preverProprio(mediaPorDia,diaAtual);
}

/**
* Metodo sem retorno que faz o calculo da previsao de consumo total das tomadas.
*/

void fazerPrevisaoTotal(){
	/**
	* atribuicao do retorno da chamada Previsor::preverTotal(tomadasExternas, numTomadasExternas, previsaoPropria)
	* para a variavel previsaoGeral
	*/
	previsaoGeral = Previsor::preverTotal(tomadasExternas,numTomadasExternas,previsaoPropria);
}

/**
* Metodo sem retorno que adiciona uma tomada ao array de tomadas externas.
* @param infoTomadas t
*/

void adicionarTomada(infoTomadas t){
	if(numTomadasExternas <= 20){
		/**
		* configura a tomada adicionada como atualizada
		*/
		tomadasExternas[numTomadasExternas].atualizado = true;
		/**
		* configura o endereco da tomada nova
		*/
		tomadasExternas[numTomadasExternas].address = t.address;
		/**
		* configura a previsao da tomada nova
		*/
		tomadasExternas[numTomadasExternas].previsao = t.previsao;
		/**
		* configura a prioridade da tomada nova
		*/
		tomadasExternas[numTomadasExternas].prioridade = t.prioridade;
		/**
		* configura se é para pular ou não a nova tomada
		*/
		tomadasExternas[numTomadasExternas].pule = false;
		/**
		* incrementa o numero de tomadas externas.
		*/
		numTomadasExternas++; 
	}
}

/**
* Metodo sem retorno para imprimir o array de tomadas externas.
*/

void imprimirArray(){
	/**
	* laco utilizado para percorrer o array de tomadas externas.
	*/
	for(int i = 0; i < numTomadasExternas; i++){
		cout<<"##################\n";
		/**
		* indica o numero da tomada no array de tomadas externas
		*/
		cout<<"tomada " << i << "\n";
		/**
		* indca o endereco da tomada
		*/
		cout<<"endereco: " << tomadasExternas[i].address << "\n";
		/**
		* indica a previsao de consumo da tomada
		*/
		cout<<"previsao: " << tomadasExternas[i].previsao<< "\n";
		/**
		* indica a prioridade da tomada
		*/
		cout<<"prioridade: " << tomadasExternas[i].prioridade << "\n";
		/**
		* indica se a tomada esta atualizada ou nao
		*/
		cout<<"atualizado: " << tomadasExternas[i].atualizado << "\n";
		/**
		* indica se pode ou não pular a tomada.
		*/
		cout<<"pule: " << tomadasExternas[i].pule << "\n";
	}
}

/**
* Construtor da classe Gerente
* @param TomadaComSensot t
* @param tomada t
* @param msngr this
*/

Gerente(TomadaComSensor t) :tomada(t), msngr(this){
	/**
	* incializa o numero de tomadas externas como zero
	*/
	numTomadasExternas = 0;
	/**
	* incializa o numero de dias como zero
	*/
	diaAtual = 0;
	/**
	* incializa a hora atual como zero
	*/
	horaAtual = 0;
	/**
	* incializa a previsaoPropria como zero
	*/
	previsaoPropria = 0;
	/**
	* incializa a previsaoGeral como zero
	*/
	previsaoGeral = 0;
}

/**
* atributos e metodo privados da classe gerente
*/
private:
	/**
	* array de 20 posicoes para armazenas as tomadas externas
	*/
	infoTomadas tomadasExternas[20];
	/**
	* inteiro para armazenar o numeto total de tomadas externas
	*/
	int numTomadasExternas;
	/**
	* array de 24 possicoes para indicar a meida por hora do dia
	*/
	double mediaPorHora[24];
	/**
	* array de 30 posicoes para indicar a media por dia do mes
	*/
	double mediaPorDia[30];
	/**
	* inteiro para indicar o dia atual do mes
	*/
	int diaAtual;
	/**
	* inteiro para indicar a hora atual do dia
	*/
	int horaAtual;
	/**
	* double para armazenar a previsao da propria tomada
	*/
	double previsaoPropria;
	/**
	* double para armazenar a previsao media de todas as tomadas
	*/
	double previsaoGeral;
	/**
	* Objeto TomadaComSensor utilizada ao longo do gerenciador 
	*/
	TomadaComSensor tomada;
	/**
	* Objeto Mensageiro utilizado para enviar ou receber mensagens das demais tomadas
	*/
	Mensageiro msngr;

};

//------------------------------------Mensageiro-----------------------------------

/**
* Metodo com retorno inteiro utilizado pelo mensageiro para receber mensagens
* das demais tomadas
* @return int
*/
int Mensageiro::receberViaNIC(){
	/**
	* Variavel utilizada para armazenar informacoes sobre a placa de rede
	*/
	NIC nic;
	/**
	* Variavel utilizada para armazenar o endereco da placa de rede.
	*/
	NIC::Address src;
	/**
	* character utilizado para indicar o protocolo utilizado na comunicacao via rede
	*/
	unsigned char prot;
	/**
	* Struct que armazena as informacoes sobre a tomada
	*/
	infoTomadas meg;
	while(1){
	/**
	* laco que faz espera ocupada para receber mensagens de outras tomadas
	*/
		while(!nic.receive(&src, &prot,&meg,sizeof(meg)) >0);
		/**
		* verifica se a mensagem recebida tem endereco diferente de zero
		*/
		if(meg.address != 0){
		/**
		* caso endereco seja diferente de zero, chama a funcao para receber a
		* mensagem.
		*/
			gerente->receberMensagem(meg);
		}
	/**
	* configura o endereco da tomada que enviou a mensagem como zero
	*/
		meg.address = 0;
	}
	/**
	* retorna um int
	*/
	return 0;
}

/**
* Construtor da classe mensageiro
* @param Gerente gnt
*/

Mensageiro::Mensageiro(Gerente * gnt){
	/**
	* atribui a gerente o gerente recebido por parametro
	*/
	gerente = gnt;
	/**
	* cria uma thread para monitorar o recebimento de mensagens via rede.
	*/
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
