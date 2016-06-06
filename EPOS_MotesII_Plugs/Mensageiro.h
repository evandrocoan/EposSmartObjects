#include <Mensagem.h>
#include <Gerente.h>
#include <nic.h>

__USING_SYS

class Mensageiro{

 private:
  pthread_t threadReceber;
  NIC nic;

 public:
  Mensageiro();

  void enviarViaNIC(Mensagem *msg, const add){
    nic.send(&add, (NIC::Protocol) 1, msg, sizeof(msg));
  }

  void receberViaNIC(){
    while (true) {
      if(!nic.received())

    }
  }

  void enviaUARD(Mensagem msg){
    //TO-DO
  }

  void recebeUARD(){
    //TO-DO
  }
}
