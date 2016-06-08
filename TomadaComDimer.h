#import "Mensagem.h"
#import "Previsor.h"
#import "Mensageiro.h"
#import "TomadaSimples.h"
#import "Gerente.h"

__USING_SYS

class TomadaComDimer : public TomadaSimples{
	protected:
		int nivelDimer;
		bool estaDimerizando;

	public:
		int getNivelDeForca(){
			return nivelDimer;
		}
		void setNivelDeForca(int nivelDeForca){
			nivelDimer = nivelDeForca;
		}
		bool getEstaDimerizando(){
			return estaDimerizando;
		}
		void setEstaDimerizando(bool estaDimerizand){
			estaDimerizando = estaDimerizand;
		}

	TomadaComDimer(){
		estaDimerizando = false;
		nivelDimer = 100;
	}
}
