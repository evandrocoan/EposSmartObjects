#include <nic.h>
#include <utility/ostream.h>
#include <utility/random.h>
#include <periodic_thread.h>
#include <alarm.h>
#include <clock.h>

__USING_SYS

OStream cout;

const unsigned int GPIO_BASE =0x80000000;
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;

NIC * nic;

struct infoTomadas {
	int previsao; 	/**< Previsao do consumo de energia. */
	int address;  	/**< Endereco da tomada. */
	int prioridade;	/**< Prioridade da tomada */
	int tipo;	/**< Tipo da tomada (dimerizavel ou nao) */
} packed;

enum acao {ATUALIZACAO = 0, NONE = 1, DESLIGAR = 2, DIM = 3, LIGAR = 4}; /**< enumeracao dos tipos de mensagens 										que podem ser enviadas */
//---------------------------------Declaracao-Gerente-----------------------------


class Gerente;

//----------------------------------Mensageiro-Declaracao-------------------------

class Mensageiro{

public:

	/**
	 Metodo responsavel por enviar uma mensagem BROADCAST para todas as tomadas.
	 A mensagem enviada sera os atributos da tomada onde o destinatario decide
	 o que fazer com elas.
	 @param msg: informacoes da tomada remetente.
	*/
	void enviarViaNIC(infoTomadas msg){
		int r;
		if((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof(msg))) != 11){
			cout<<"failed\n";
		}
		//nic->reset();
		cout<<nic->statistics().tx_packets << "  pacotes enviados\n";
	}

	/**
	* Metodo responsavel por receber uma mensagem BROADCAST.
	*/
	static int receberViaNIC();	


	/**
	*	Construtor da classe Mensageiro.
	* @param gnt: ponteiro do gerente que ira chamar os metodos da classe mensageiro.
	*/
	Mensageiro(Gerente * gnt);

private:
	static Gerente *gerente; /**< Ponteiro do gerente que ira chamar os metodos do mensageiro. */

};

Gerente * Mensageiro::gerente;   /**< Instancia do módulo de radio */

//----------------------------------Previsor---------------------------------------

/**
*	Classe responsavel por fazer as previsoes de consumo de uma dada tomada.
*/
class Previsor{

public:

/**
* Preve o consumo de energia do dia.
* @param mediaPorHora: array com o consumo medio de cada hora do dia.
* @param horaAtual: hora atual do dia.
* @return soma: previsao de consumo do dia.
*/
static double preverDia(double mediaPorHora[24],int horaAtual){
	double soma = 0;
	for(int i = 0; i < horaAtual+1;i++){
		soma += mediaPorHora[i];
	}
	double media = soma / horaAtual;
	soma += media * (24 - horaAtual);
	return soma;
}

/**
* Preve o consumo de energia mensal.
* @param mediaPorDia: array com os dias do mes atual.
* @param diaAtual: dia atual.
* @return soma: previsao do consumo mensal.
*/
static double preverProprio(double mediaPorDia[30], int diaAtual){
	double soma = 0;
	for(int i = 0; i < diaAtual; i++){
		soma += mediaPorDia[i];
	}
	 
	double media = soma/diaAtual;
	
	soma += media * (30 - diaAtual);
	
	return soma;
}

/**
* Preve o consumo total de todas as tomadas juntas.
* @param *tomadas: ponteiro do struct das tomadas.
* @param sizeTomadas: numero de tomadas no sistema.
* @param previsaoPropria: previsao da propria tomada que esta fazendo o calculo.
* @return preverTotal: previsao total de todas as tomadas juntas.
*/
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

/**
* Classe para testar os LEDs do EPOS.
*/

class ledTest{

public:

/**
* Metodo que liga ou desliga o LED da placa.
* @param led: led a ser alterado.
* @param on: flag que indica se o LED esta ligado ou nao.
*/
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

/**
* Classe que representa uma tomada.
*/

class Tomada{

public:

	/**
	* Verifica se a tomada esta ligada ou nao.
	* @return ligado: valor que indica se a tomada esta ligada ou nao.
	*/
	bool estaLigado(){
		return ligado;
	}

	/**
	* Liga a tomada.
	*/
	void ligar(){
		ligado = true;
		ledTest::turn_led(23,true);
	}

	/**
	* Desliga a tomada.
	*/
	void desligar(){
		ligado = false;
		ledTest::turn_led(23,false);
	}

	/**
	* Construtor da tomada. Inicialmente a tomada inicia desligada.
	*/
	Tomada(){
		desligar();
	}

	/**
	* Retorna o tipo da tomada (-1 significa que esta tomada nao tem gerente)
	*/
	int getTipo(){
		return -1;
	}

protected:
	bool ligado;  /**< Flag que indica se a tomada esta ligada ou nao. */
};

/**
* Classe que representa uma tomada com sensor.
*/
class TomadaComSensor : public Tomada{
public:

	/**
	* Busca o consumo atual da tomada.
	* @return consumo: consumo atual de energia da tomada. Caso ela estiver desligada
	* retorna 0.
	*/
	double getConsumo(){
		//double consumo = sensor->sample();
		double consumo = (double)((Pseudo_Random::random() * Pseudo_Random::random()) % (int)consumoMaximo);
		if(consumo < consumoMinimo && consumo > 0){
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

	/**
	* Retorna o tipo da tomada (0 significa que ela nao dimeriza)
	*/
	int getTipo(){
		return 0;
	}

	/**
	* Busca o endereco da tomada.
	* @return address: endereco da tomada.
	*/
	int getAddress(){
		return address;
	}

	/**
	* Busca o consumo maximo que a tomada ja consumiu.
	* @return consumoMaximo: consumo maximo que a tomada ja consumiu.
	*/
	double getConsumoMaximo(){
		return consumoMaximo;
	}

	/**
	* Busca o consumo minimo que a tomada ja consumiu.
	* @param consumoMinimo: consumo minimo que a tomada ja consumiu.
	*/
	double getConsumoMinimo(){
		return consumoMinimo;
	}

	/**
	* Busca o consumo medio de energia já consumida pela tomada.
	* @return consumo medio de energia já consumida pela tomada.
	*/
	double getMediaDeConsumo(){
		return (consumoMaximo + consumoMinimo)/2;
	}

	/**
	* Retorna o estado atual do PowerSaving.
	* @return PowerSaving: retorna o estado atual do PowerSaving.
	*/
	bool getPowerSavingMode(){
		return PowerSaving;
	}

	/**
	* Modifica o estado do PowerSaving para o indicado atraves do parametro.
	* @param on: valor booleano que ira alterar o estado do PowerSaving.
	*/
	void setPowerSavingMode(bool on){
		PowerSaving = on;
	}

	/**
	* Modifica o limite do modo Power Saving
	* @param limit: novo valor do limite
	*/
	void setPowerSavingLimit(int limit = 0){
		limite = limit;
	}

	/** 
	* Retorna o valor do limite do modo Power Saving
	* @return limite: o valor do limite do modo Power Saving
	*/
	int getPowerSavingLimit(){
		return limite;
	}

	/**
	* Busca a prioridade da tomada.
	* @return prioridade: prioridade da tomada.
	*/
	int getPrioridade(){
		return prioridade;
	}

	/**
	* Altera a prioridade da tomada para o valor indicado atraves do parametro.
	* @param prio: valor a ser atribuido a prioridade da tomada.
	*/
	void setPrioridade(int prio = 0){
		prioridade = prio;
	}

	/**
	* Construtor da Tomada com Sensor.
	*/
	TomadaComSensor(){
		PowerSaving = false;
		prioridade = 0;
		consumoMaximo = 1220;
		consumoMinimo = 255000;
		address = 0x4;
		limite = 0;
	}
protected:
	int prioridade;	/**< Prioridade da tomada. */
	double consumoMaximo; /**< Consumo maximo ja consumido pela tomada. */
	double consumoMinimo; /**< Consumo minimo ja consumido pela tomada. */
	bool PowerSaving;	/**< Flag que indica se a tomada esta em PowerSaving ou nao. */
	int address;	/**< Endereco da tomada. */
	double limite; /**< Valor limite que a tomada pode consumir de energia. */
};

/**
* Classe da tomada dimer
*/
class TomadaDimer : public Tomada{

public:

/**
* Metodo que dimeriza a tomada
* @param porcentagem: porcentagem de dimerizacao
*/
void dim(int porcentagem){
	if(porcentagem < 100 && porcentagem > 0){
		dimPorcent = porcentagem;
	} else {
		cout<< "porcentagem invalida\n";
	}
}

/**
* Metodo para cuidar da demonstracao pelo led
*/
void dimLed(){
}

/**
* Retorna o tipo da tomada
* @return: retorna o tipo da tomada (-1 pois esta tomada nao possui sensor)
*/
int getTipo(){
	return -1;
}

/** 
* Construtor da tomada com dimer
*/
TomadaDimer() {//: dimerThread(&dimLed){
	dimPorcent = 0;
}

private:
	int dimPorcent; /**< Guarda a porcentagem atual de dimerizacao */
	//Thread dimerThread;

};


/**
* Classe da tomada com sensor e dime
*/
class TomadaTop : public TomadaComSensor , public TomadaDimer{

/**
* Retorna o tipo da tomada
* @return: Tipo da tomada
*/
int getTipo(){
	return 1;
}

/**
* construtor da tomada com sensor e dimer
*/
TomadaTop(){
}

};

//--------------------------Gerente-----------------------------------------
/**
	Classe responsavel por gerenciar a tomada e manter atualizada.
*/
class Gerente{

public:
/**
	Metodo sem retorno para enviar atualizacoes sobre a tomada.
	@param int tipo
*/
void enviarMensagem(int tipo){
	mediaPorDia[diaAtual++] = tomada.getConsumo();
	if(diaAtual > 30){diaAtual = 0;}
	fazerPrevisaoPropria();
	msg.previsao = previsaoPropria;
	msg.address = tomada.getAddress();
	msg.prioridade = tomada.getPrioridade();
	msg.tipo = tipo;

	for (volatile unsigned long i = 0; i <= msg.address * 50 * (Traits<Machine>::CLOCK / 1000UL); i++);

	msngr.enviarViaNIC(msg);
	clean();
}
/**
	Metodo sem retorno para receber atualizacoes sobre as demais tomadas da rede.
	@param inforTomadas msg
*/
void receberMensagem(infoTomadas msg){
	for(int i = 0; i < numTomadasExternas; i++){
		if(msg.address == tomadasExternas[i].address){
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

/**
* Metodo sem retorno que limpa as posicoes da mensagem para que sejam reescritas
*/
void clean(){
	msg.previsao = 0;
	msg.address = 0;
	msg.prioridade = 0;
	msg.tipo = 0;
}


/**
	Metodo sem retorno que faz o calculo da previsao de consumo.
*/
void fazerPrevisaoPropria(){
	previsaoPropria = Previsor::preverProprio(mediaPorDia,diaAtual);
}

/**
	Metodo sem retorno que faz o calculo da previsao de consumo total das tomadas.
*/
void fazerPrevisaoTotal(){
	previsaoGeral = Previsor::preverTotal(tomadasExternas,numTomadasExternas,previsaoPropria);
}

/**
	Metodo sem retorno que adiciona uma tomada ao array de tomadas externas.
	@param infoTomadas t
*/

void adicionarTomada(infoTomadas t){
	if(numTomadasExternas <= 20){ 
		tomadasExternas[numTomadasExternas].address = t.address;
		tomadasExternas[numTomadasExternas].previsao = t.previsao;
		tomadasExternas[numTomadasExternas].prioridade = t.prioridade;
		tomadasExternas[numTomadasExternas].tipo = t.tipo;

		numTomadasExternas++; 
	}
}

/**
	Metodo sem retorno para imprimir o array de tomadas externas.
*/
void imprimirArray(){  //function for debug purposes
	for(int i = 0; i < numTomadasExternas; i++){
		cout<<"##################\n";
		cout<<"tomada " << i << "\n";
		cout<<"endereco: " << tomadasExternas[i].address << "\n";
		cout<<"previsao: " << tomadasExternas[i].previsao<< "\n";
		cout<<"prioridade: " << tomadasExternas[i].prioridade << "\n";
	}
}

/**
* Metodo principal que calcula as acoes que uma tomada ira tomar
*/
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
			return;
		} else {
			if(tomada.getConsumoMaximo() + previsaoGeral > tomada.getPowerSavingLimit()){
				enviarMensagem(NONE);
				return;
			} else {
				tomada.ligar();
				enviarMensagem(LIGAR);
				return;
			}
		}
	}

	if(myTurn){
		if(tomada.getConsumo() < tomada.getMediaDeConsumo()){
			// tipo 0 = sem dimer
			if(tomada.getTipo() == 0){
				tomada.desligar();
				enviarMensagem(DESLIGAR);
				return;
				
			} else {
				//tomada.dim(50);
				enviarMensagem(DIM);
				return;
			}
		}
			
	}
}

//*
/**
* Metodo que a thread de gerencia deveria executar
* nao implementado por motivos de incompatibilidade do timer com o NIC
*/
static int start(Gerente *g){
	while(1){
		cout<<"start2\n";
		g->enviarMensagem(ATUALIZACAO);
		//while(!g->checarAtualizados());
		//g->tomadaInteligente();
		//Alarm::delay(1000000);
		for (volatile unsigned long i = 0; i <= 50 * (Traits<Machine>::CLOCK / 1000UL); i++);
	
	//Periodic_Thread::wait_next(); //não funciona
	}
	return 0;
}
//*/

/**
	Construtor da classe Gerente
	@param TomadaComSensot t
	@param tomada t
	@param msngr this
*/
Gerente(TomadaComSensor t) :tomada(t), msngr(this){
	numTomadasExternas = 0;
	diaAtual = 0;
	horaAtual = 0;
	previsaoPropria = 0;
	previsaoGeral = 0;
	/*
	Thread *monitor;
	monitor = new Thread( &start,this );
	*/
}

private:
infoTomadas msg;			/**< Referencia ao struct da mensagem */
infoTomadas tomadasExternas[20];	/**< Array de 20 posicoes para armazenas as tomadas externas. */
int numTomadasExternas;			/**< Inteiro para armazenar o numeto total de tomadas externas. */
double mediaPorHora[24];		/**< Array de 24 posicoes para indicar a media por hora do dia. */
double mediaPorDia[30];			/**< Array de 30 posicoes para indicar a media por dia do mes. */
int diaAtual;				/**< Inteiro para indicar o dia atual do mes. */
int horaAtual;				/**< Inteiro para indicar a hora atual do dia. */
double previsaoPropria;			/**< Double para armazenar a previsao da propria tomada. */
double previsaoGeral;			/**< Double para armazenar a previsao media de todas as tomadas. */


TomadaComSensor tomada;			/**< Referencia a tomada gerenciada */
Mensageiro msngr;			/**< Referencia ao mensageiro */

};

//------------------------------------Mensageiro-----------------------------------
/**
	Metodo com retorno inteiro utilizado pelo mensageiro para receber mensagens
	das demais tomadas
	@return int: retorna 0 apos receber mensagem via NIC.
*/
int Mensageiro::receberViaNIC(){
		//NIC nic;		
		NIC::Address src;
		unsigned char prot;
		infoTomadas meg;
		while(1){
			while(!nic->receive(&src, &prot,&meg,sizeof(meg)) >0);
			if(meg.address != 0){
				gerente->receberMensagem(meg);
			}
			meg.address = 0;
		}
		return 0;
	}	

/**
	Construtor da classe mensageiro
	@param Gerente gnt: gerente que sera associado ao mensageiro.
*/
Mensageiro::Mensageiro(Gerente * gnt){
		gerente = gnt;
		Thread *thread;
		thread = new Thread( &receberViaNIC );
}



//------------------------------------Main-------------------------------------------
/**
	Classe principal do programa.
*/

int main(){

	nic = new NIC();	

	TomadaComSensor t;
	t.ligar();
	Gerente g(t);

	t.setPrioridade(10);
	cout<< "\n"<< t.getPrioridade() << "\n";
	cout<< t.estaLigado() << "\n";
	
	while(1){
		cout<<"start3\n";
		g.enviarMensagem(NONE);
		//g->tomadaInteligente();
		//Alarm::delay(1000000);
		for (volatile unsigned long i = 0; i <= 5 * 50 * (Traits<Machine>::CLOCK / 1000UL); i++);

		//ultra mega gambi
	}

}
