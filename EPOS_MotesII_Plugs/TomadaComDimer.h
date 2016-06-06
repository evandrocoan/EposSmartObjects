#import <Mensagem.h>
#import <Previsor.h>
#import <Tomada.h>

__USING_SYS

class TomadaComDimer{
	private:
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