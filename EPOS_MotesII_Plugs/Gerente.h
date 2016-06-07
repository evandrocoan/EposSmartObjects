#include "Mensageiro.h"
#include "Mensagem.h"
#include "Previsor.h"
#include "Tomada.h"
#include <nic.h>
#include <clock.h>

__USING_SYS

class Gerente{

struct {
int previsao;
NIC::Address endereco;
int prioridade;
Mensagem::Tipo tipo;
} infoTomada;

private:
infoTomada* tomadasExternas[0];
int numTomadasExternas = 0;
double mediaDoDia[24];
double mediaDiariaPorMes[30];
int diaAtual = 0;
int horaAtual = 0;
double previsaoPropria = 0;
double previsaoTotal = 0;
Tomada* tomada;
Thread* monitor;
Clock* clock;
NIC nic;
NIC::Address address = nic.address;
bool appRun = true;

public:

void send(int destino, NIC::Address origem, Tipo tipo, string msg);
void receive();
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
		if(tomada->Tipo() > 1){
			if(tomada->getConsumo() < media()){
				if(tomada->Tipo() == 2){
					tomada->turnOff();
					send(0,address,Mensagem::Acao,"Off");
					
				} else {
					tomada->dim();
					send(0,address,Mensagem::Acao,"Dim");
				}
			}
		}	
	}
}
int media(){
	int soma = tomada->getMaiorConsumo() + tomada->getMenorConsumo();
	return soma/2;
}
void start(){
	horaAtual = (int)clock->now()/3600;
	diaAtual = clock->date()->day();
	while(appRun){
		if((int)clock->now()/3600 == horaAtual + 1){
			horaAtual = (int)clock->now()/3600;
			mediaDoDia[horaAtual] = tomada->getConsumo();
			mediaDiariaPorMes[diaAtual] = Previsor::preverDia;
			previsaoPropria = Previsor::preverProprio(mediaDiariaPorMes, diaAtual);
			send(0,address,
			
			
			
	}
	
}
Gerente(Toamda* tomada){
	this->tomada = tomada;
	monitor = new Thread(&start):
}
}
