#import <Mensagem.h>
#import <Previsor.h>
#import <Tomada.h>

__USING_SYS

class TomadaSimples{
	private:
		bool ligado;

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
	}
}