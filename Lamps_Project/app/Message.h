#ifndef MENSAGEM

#include "ofstream.h"
#include "ifstream.h"
namespace pj{

class Message {




private:
int destinatario;
int remetente;
int tipo;

int size;
char* dados;

public:
Message(){
}
Message(int destino, int origem, int tipo, char* dados, int size)
  :  destinatario(destino), remetente(origem), tipo(tipo), size(size)
{
    this->dados = new char[size];
    for (int i=0;i<size;i++){
        this->dados[i]= dados[i];
    }
}
~Message(){
    delete(dados);
}
Message( const Message& m):
    Message(m.destinatario, m.remetente, m.tipo, m.dados, m.size ) {

}


void* getRest(){
    return dados;
}
int getDataSize(){
    return size;
}

int getDest(){
	return destinatario;
}

int getOrig(){
	return remetente;
}

int getTipo(){
	return tipo;
}
friend ofstream& operator<<(ofstream& of,const Message& m){
    int msgSize = sizeof(int)*4 + m.size;/// (dest,orig ,tipo, tam)*4 + tam;
    char packet[msgSize];

    int pointer =0;
    packet[pointer] = m.destinatario;

    pointer+= sizeof(int);
    packet[pointer] = m.remetente;

    pointer+= sizeof(int);
    packet[pointer] = m.tipo;

    pointer+= sizeof(int);
    packet[pointer] = m.size;

    pointer+= sizeof(int);

    for (int i=0;i<m.size;i++){
        packet[pointer] = m.dados[i];
        pointer++;
    }

    of<<packet;
    return of;
}

friend ifstream& operator>>(ifstream& is, Message& m){


}


};

}

#endif
