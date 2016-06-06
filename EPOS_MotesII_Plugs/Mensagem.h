#import <string.h>

__USING_SYS

class Mensagem {

enum Tipo {Descobrimento = 0, Atualizacao = 1, Acao = 2};

private:
string msg;
int destinatario;
int remetente;
Tipo tipo;

public:
Mensagem(int destino, int origem, Tipo tipo, string msg){
	this->destinatario = destino;
	this->remetente = origem;
	this->tipo = tipo;
	this->msg = msg;
}

string getMsg(){
	return msg;
}

int getDest(){
	return destinatario;
}

int getOrig(){
	return remetente;
}

Tipo getTipo(){
	return tipo;
}
