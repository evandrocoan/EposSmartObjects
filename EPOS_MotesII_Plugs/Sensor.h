#include <adc.h>
#include <random.h>

__USING_SYS

//PS haverão duas funcoes aqui, uma para se o sensor funcionar e outra se não
class Sensor{
ADC *adc = new ADC();

public static double getConsumoSensor(){
adc->setConfig(7);
//consumo medido em corrente (amperes)
//watts = amperes x voltagem
//aqui retorna o consumo em watts

while(!adc->finished());
return adc->sample();
}

public static double getConsumoRandom(double maiorLimite, double menorLimite,int dimPorcentage){

int random = Pseudo_Random::random(0);
int floater = Pseudo_Random::random(2);
while((random + (floater * 0,01)) > maiorLimite || (random + (floater * 0,01)) < menorLimite){
	random = Pseudo_Random::random(1);
}

return (double)(random + (floater * 0,01));
}









}
