#import <Mensagem.h>
#import <Previsor.h>
#import <Tomada.h>

__USING_SYS

class Gerente{

struct {
int previsao;
int endereco;
int prioridade;
Mensagem::Tipo tipo;
} infoTomada;

private:
infoTomada* tomadasExternas[0];
int numTomadasExternas = 0;
int mediaDoDia[24];
int mediaDoPeríodo[30];
int diaAtual = 0;
int horaAtual = 0;
double previsaoPropria = 0;
double previsaoTotal = 0;
Tomada* tomada;

public:

void send(Mensagem msg);
void receive(Mensagem msg);
void fazerPrevisaoDestaTomada(){
	previsaoPropria = Previsor::preverProprio(mediaPeriodo, diaAtual);
}
void fazerPrevisaoTotal(){
	previsaoTotal = Previsor::preverTotal(tomadasExternas, numTomadasExternas, previsaoPropria);
}
void tomadaInteligente(){
	if(previsaoTotal > tomada->getLimite()){
		bool myTurn = true;
		for(int i = 0; i < numTomadasExternas;i++){
			if(tomadasExternas[i]->prioridade < tomada->prioriade()){
				if(tomadasExternas[i]->acao == NONE){
					myTurn = false;
					break;
				} 
			}
		}
	}

	if(myTurn){
		if(tomada->Tipo() == 0){
			tomada->turnOff();
		} else
		if(tomada->Tipo() == 1){
			tomada->dim();
		}
	}
}
Gerente(){
	tomada = new Tomada();
}
		

