#include <nic.h>
#include <utility/ostream.h>
#include <utility/random.h>
#include <alarm.h>
#include <clock.h>

__USING_SYS

OStream cout;

const unsigned int GPIO_BASE =0x80000000; /**< For debug purposes. Endereco de memoria para minipular os LEDs*/
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;	/**< For debug purposes. Endereco de memoria para minipular os LEDs*/
const unsigned int GPIO_PAD_DIR0  = GPIO_BASE + 0x00;	/**< For debug purposes. Endereco de memoria para minipular os LEDs*/

/**
	Struct com informacoes (atributos) de cada tomada.
*/
typedef struct {
	int previsao;	/**< Previsao do seu consumo de energia. */
	int address;	/**< Endereco da tomada. */
	int prioridade;	/**< Prioridade da tomada. */
	int tipo;	/**< Tipo da tomada (dimerizavel, ou nao, por exemplo). */
	bool pule;
	bool atualizado;	/**< Flag para verificar se o valor da previsao atual esta atualizado. */
} infoTomadas;

class Gerente;

/**
	Classe responsavel por enviar as mensagens para outras tomadas.
*/
class Mensageiro{

public:

	/**
	 Metodo responsavel por enviar uma mensagem BROADCAST para todas as tomadas.
	 A mensagem enviada sera os atributos da tomada onde o destinatario decide
	 o que fazer com elas.
	 @param msg: informacoes da tomada remetente.
	*/
	void enviarViaNIC(infoTomadas msg){
		NIC nic;
		nic.send(NIC::BROADCAST, (NIC::Protocol)1, &msg,sizeof(msg));
		Alarm::delay(10000);
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

Gerente * Mensageiro::gerente;	/**<  */

//----------------------------------Previsor---------------------------------------

/**
*	Classe responsavel por fazer as previsoes de consumo de uma dada tomada.
*/
class Previsor{

public:

/**
*	Preve o consumo de energia do dia.
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
*	Preve o consumo total de todas as tomadas juntas.
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
		ligado = false;
		ledTest::turn_led(23,false);
	}

protected:
	bool ligado; /**< Flag que indica se a tomada esta ligada ou nao. */
};

/**
* Classe que representa uma tomada com sensor.
*/
class TomadaComSensor : public Tomada{
public:

	/**
	* Busca o consumo atual da tomada.
	* @return consumo: consumo atual de energia da tomada. Caso ela estiver desligada
	* retorna -1.
	*/
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
	void setPrioridade(int prio){
		prioridade = prio;
	}

	/**
	* Construtor da Tomada com Sensor.
	*/
	TomadaComSensor(){
		PowerSaving = false;
		prioridade = 15;
		consumoMaximo = 1220;
		consumoMinimo = 255000;
		address = 0x2;
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
	infoTomadas msg;
	fazerPrevisaoPropria();
	msg.previsao = previsaoPropria;
	msg.address = tomada.getAddress();
	msg.prioridade = tomada.getPrioridade();
	msg.tipo = tipo;
	msngr.enviarViaNIC(msg);
}

/**
	Metodo sem retorno para receber atualizacoes sobre as demais tomadas da rede.
	@param inforTomadas msg
*/
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
	tomadasExternas[numTomadasExternas].atualizado = true;
	tomadasExternas[numTomadasExternas].address = t.address;
	tomadasExternas[numTomadasExternas].previsao = t.previsao;
	tomadasExternas[numTomadasExternas].prioridade = t.prioridade;
	tomadasExternas[numTomadasExternas].pule = false;

	numTomadasExternas++;
	}
}

/**
	Metodo sem retorno para imprimir o array de tomadas externas.
*/
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
}

private:
infoTomadas tomadasExternas[20]; /**< Array de 20 posicoes para armazenas as tomadas externas. */
int numTomadasExternas;	/**< Inteiro para armazenar o numeto total de tomadas externas. */
double mediaPorHora[24];	/**< Array de 24 posicoes para indicar a media por hora do dia. */
int diaAtual;	/**< Inteiro para indicar o dia atual do mes. */
double mediaPorDia[30];	/**< Array de 30 posicoes para indicar a media por dia do mes. */
int horaAtual;	/**< Inteiro para indicar a hora atual do dia. */
double previsaoPropria;	/**< Double para armazenar a previsao da propria tomada. */
double previsaoGeral;	/**< Double para armazenar a previsao media de todas as tomadas. */

TomadaComSensor tomada; /**< Objeto TomadaComSensor utilizada ao longo do gerenciador. */
Mensageiro msngr;	/**< Objeto Mensageiro utilizado para enviar ou receber mensagens das demais tomadas. */

};

//------------------------------------Mensageiro-----------------------------------
/**
	Metodo com retorno inteiro utilizado pelo mensageiro para receber mensagens
	das demais tomadas
	@return int: retorna 0 apos receber mensagem via NIC.
*/
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
