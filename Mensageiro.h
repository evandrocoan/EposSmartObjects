#include "Mensagem.h"
#include <nic.h>

#ifndef MENSAGEIRO_H
#define MENSAGEIRO_H

__USING_SYS

class Mensageiro{

 private:
  	Thread* threadReceber;
 	
	Gerente *gnt;

 public:
  Mensageiro(Gerente *gnt){
	threadReceber = new Thread(&receberViaNIC);
	this->gnt = gnt;
  }

  void enviarViaNIC(Mensagem *msg, const add){
    NIC nic;
    nic.send(&add, (NIC::Protocol) 1, msg, sizeof(msg));
  }

  void receberViaNIC(){
	NIC nic;
	string msg;
	int prot;
	int remetente;

    	while (true) {
      		if(!nic.receive(&remetente,&prot,&msg,sizeof(msg))>0){
			gnt->receive(msg);
		}	
    	}
  }

  void enviaUART(Mensagem msg){
    //TO-DO
  }

  void recebeUART(){
    //TO-DO
  }
}
#endif
