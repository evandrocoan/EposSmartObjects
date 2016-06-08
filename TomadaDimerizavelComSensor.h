#import "Mensagem.h"
#import "Previsor.h"
#import "Mensageiro.h"
#import "Gerente.h"
#import "TomadaComSensor.h"
#import "TomadaComDimer.h"
#import <random.h>

__USING_SYS

class TomadaDimerizavelComSensor : public TomadaComSensor,public TomadaComDimer{

	TomadaDimerizavelComSensor(){
	}
}
