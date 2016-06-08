#import "Mensagem.h"
#import "Mensageiro.h"
#import "Previsor.h"

__USING_SYS

class TomadaSimples{
	protected:
		bool ligado;
		Mensageiro *msgr;		

	public:
		bool getLigado(){
			return ligado;
		}
	void setLigado(bool Ligado){
		ligado = Ligado;
	}

	int getTipoDeTomada(){
		return 0;
	}
	TomadaSimples(){
		ligado = false;
		msgr = new Mensageiro();
	}
}
