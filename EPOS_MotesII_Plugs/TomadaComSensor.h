#include "Mensagem.h"
#include "Previsor.h"
#include "Tomada.h"

__USING_SYS

class TomadaComSensor{
	private:
		double menorConsumo;
		double maiorConsumo;
		double ultimaMedicao;
		bool powerSavingMode;
		int prioridade;

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
		menorConsumo = 1000000000000000000000000;
		maiorConsumo = -1111100000000000000000000;
		powerSavingMode = false;
		prioridade = 0;
	}
}