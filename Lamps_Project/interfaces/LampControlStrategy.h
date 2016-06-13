/**
* This work is licensed under the EPOS Software License v1.0.
* A copy of this license is available at the EPOS system source tree root.
* Note that EPOS Software License applies to both source code and executables.
*
*  You should have received a copy of the EPOS Software License v1.0 along
*  with this program. If not, see
*  <http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0>.
*
******************************************************************************
*/


#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>


/**
 * DO NOT MODIFIY IT!
 * 
 * @see The Class LampBoard main include declaration/explanation.
 */
#include <classes/Led.cc>



typedef int (*LightVariationFunc)(int);

const int lightMax = 4000;


//Should use constant here that set whether user wants a darker or a brighter environment.
const int lightMin = 2400;

//Max - ( Read - Min )
// = Max + Min - Read
ADC adc(ADC::SINGLE_ENDED_ADC7);

int DefaultLightVal( int val )
{
	return val;
}

int Alternative1( int val )
{
	static int transformValue = lightMax + lightMin;
	int temp = transformValue - adc.get();
	double perc = ((double) temp) / lightMax;
	int parcial = val - 100 * perc;

	if (parcial < 0)
		return 0;
	return parcial;
}

int Alternative2( int val )
{
	static int transformValue = lightMax + lightMin;
	int temp = transformValue - adc.get();
	int perc = (int) 100 * (((double) temp) / lightMax);
	int percToApply = 100 - perc;
	if(percToApply < 0)
		percToApply = 0;
	return (int) percToApply * ((double) val) / 100;
}



/**
 *  Base IO adress. GPIO Base is the base address of all IO operations.
 *  copied from memory_map.h
 */
const unsigned int GPIO_BASE = 0x80000000;

/**
 * DATA_SET is just so magical I can't express my thoughts into words.
 */
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;

/**
 * PAD_DIR0 contains the base address which contains the registers' values.
 */
const unsigned int GPIO_PAD_DIR0 = GPIO_BASE + 0x00;


/**
 * Only LEDs 0 to 2 (RGB) are used
 */
const unsigned int MAX_LEDS_ALLOWED_TO_BE_USED = 3;

/**
 * Please save yourself the trouble and don't read the documentation bellow. If you do
 * and still don't get it like me, then please increase the following counter to warn
 * future code revisor and save them the trouble. You have been warned.
 * count = 2
 * //4294967294 //4.294.967.294
 */
unsigned int g_effectDelay = 1e5;

/**
 * Used to stop all running threads which are running throw an while true, to perform busy wait
 * from this board devices.
 */
bool g_finishThread = false;

/**
 * Power of the lads.
 */
unsigned int power[ MAX_LEDS_ALLOWED_TO_BE_USED ]; // only leds 0 to 2 (RGB) are used

//Mutex* mutexEffect[MAX_LEDS_ALLOWED_TO_BE_USED];
bool g_effect[ MAX_LEDS_ALLOWED_TO_BE_USED ];



/**
 * This function is used to toggle the led on or off.
 *
 * @param pin        The pin number that we want to toggle
 *
 * @param on         If this is true, we toggle the bit on. If this is false, we toggle the bit off
 *
 */
void turn_led( int pin, bool on )
{
    int          bit     = pin % 32;
    unsigned int regData = GPIO_DATA_SET0 + ( ( pin >> 5 ) << 2 );
    unsigned int regPad  = GPIO_PAD_DIR0 + ( ( pin >> 5 ) << 2 );
    unsigned int value   = CPU::in32( regPad );
    
    if( on )
    {
        value |= ( 1 << bit );
    }
    else
    {
        value &= !( 1 << bit );
    }
    
    CPU::out32( regPad, value );
    CPU::out32( regData, ( 1 << bit ) );
}



/**
 * This class is the interface to an strategy to control the Lamp's objects.
 */
class LampControlStrategy
{
public:
    
    /**
     * Adds a new lamp into the controlling system.
     * 
     * @param lamp              the lamp to be updated/receive its new setting.
     * @param setting           an LampConfiguration configuration object.
     */
    virtual void addNewLamp( Lamp* lamp, LampConfiguration* setting ) = 0;
    
    /**
     * Given one lamp, changes its configuration.
     * 
     * @param lamp              the lamp to be updated/receive its new setting.
     * @param newSetting        an LampConfiguration configuration object.
     */
    virtual void setNewUserSettings( Lamp* lamp, LampConfiguration* newSetting ) = 0;
    
    
protected:
    
    /**
     * Contains the lamps this strategy is controlling.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< Lamp* > lamps;
    
};




