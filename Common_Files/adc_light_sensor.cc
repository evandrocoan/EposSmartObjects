/**
 * Structured LED EposMotesII Controlling
 *
 * This work is licensed under the EPOS Software License v1.0.
 * A copy of this license is available at the EPOS system source tree root.
 * Note that EPOS Software License applies to both source code and executables.
 *
 *  You should have received a copy of the EPOS Software License v1.0 along
 *  with this program. If not, see
 *  <http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0>.
 *
 * *****************************************************************************
 * 
 * 
 */

#include <utility/ostream.h>
#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <traits.h>

/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;

OStream cout;

typedef int (*LightVariationFunc)(int);

const int lightMax = 4000;


//Should use constant here that set whether user wants a darker or a brighter environment.
const int lightMin = 2400;

//Max - ( Read - Min )
// = Max + Min - Read

ADC adc(ADC::SINGLE_ENDED_ADC7);

int DefaultLightVal(int val) {
	return val;
}

int Alternative1(int val) {
	static int transformValue = lightMax + lightMin;
	int temp = transformValue - adc.get();
	double perc = ((double) temp) / lightMax;
	int parcial = val - 100 * perc;

	if (parcial < 0)
		return 0;
	return parcial;
}

int Alternative2(int val) {
	static int transformValue = lightMax + lightMin;
	int temp = transformValue - adc.get();
	int perc = (int) 100 * (((double) temp) / lightMax);
	int percToApply = 100 - perc;
	if(percToApply < 0)
		percToApply = 0;
	return (int) percToApply * ((double) val) / 100;
}

/**
 * Main function entry point. It need the light sensor attached to the EPOSMotes2 board to work.
 * See this image to know how to do it, or also see the EPOSMotes2 schematics:
 * <http://i.imgur.com/COifVUs.jpg>
 * <http://epos.lisha.ufsc.br/EPOSMote+II>
 */
int main() {
	const char* const PROGRAM_VERSION = "0.1";

	cout << "EposMotesII app ADC_LIGHT_SENSOR initing...";
	cout << "Program version: " << PROGRAM_VERSION;

	int i = 0;

	while( true )
    {
		Alarm::delay( 1e6 );
        
		if( i == 100 )
        {
            i = 0;
			cout << "\nHoi!! \n";
        }
        
		cout << "THE GREAT LIGHT VALUE IS: " << adc.get() << endl;
		cout << "DEFAULT: " << DefaultLightVal(30) << endl;
		cout << "ALTERNATIVE 1 : " << Alternative1(30) << endl;
		cout << "ALTERNATIVE 2 : " << Alternative2(30) << endl;
		cout << "DEFAULT: " << DefaultLightVal(60) << endl;
		cout << "ALTERNATIVE 1 : " << Alternative1(60) << endl;
		cout << "ALTERNATIVE 2 : " << Alternative2(60) << endl;
		cout << "DEFAULT: " << DefaultLightVal(100) << endl;
		cout << "ALTERNATIVE 1 : " << Alternative1(100) << endl;
		cout << "ALTERNATIVE 2 : " << Alternative2(100) << endl;
		cout << "THE WRONG GUY: " << adc.get() << endl;
        
        ++i;
	}

	cout << "Threads finished. EposMotesII app finishing";

	return 0;
}

