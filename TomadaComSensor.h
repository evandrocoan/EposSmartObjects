#import "Mensagem.h"
#import "Previsor.h"
#import "Mensageiro.h"
#import "TomadaSimples.h"
#import <random.h>

__USING_SYS

class TomadaComSensor : TomadaSimples{
	protected:
		double menorConsumo;
		double maiorConsumo;
		double ultimaMedicao;
		bool powerSavingMode;
		int prioridade;
		Gerente* grt;

	public:
		double getMenorConsumo(){
			return menorConsumo;
		}
		void setMenorConsumo(double menorConsum){
			menorConsumo = menorConsum;
		}
		double getMaiorConsumo(){
			return maiorConsumo;
		}
		void setMaiorConsumo(double maiorConsum){
			maiorConsumo = maiorConsum;
		}
		double getUltimaMedicao(){
			return ultimaMedicao;
		}
		void setUltimaMedicao(double ultimaMedica){
			ultimaMedicao = ultimaMedica;
		}
		bool getPowerSavingMode(){
			return powerSavingMode;
		}
		void setPowerSavingMode( bool powerSavingMod){
			powerSavingMode = powerSavingMod;
		}
		int getPrioridade(){
			return prioridade;
		}
		void setPrioridade(int prioridad){
			prioridade = prioridad;
		}

	TomadaComSensor(){
		ligado = false;
		menorConsumo = 0;
		maiorConsumo = 0;
		powerSavingMode = false;
		prioridade = 0;
		grt = new Gerente();
	}
}
