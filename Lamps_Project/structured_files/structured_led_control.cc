
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
#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include "RadioControl.h"
#include "RadioControl.cpp"
#include "Radio.h"

__USING_SYS;
typedef pj::Radio LampRadio;
typedef pj::RadioControl LampRadioControl;

LampRadio* myRadio;
LampRadioControl* myRadioControl;

typedef int (*LightVariationFunc)(int);

const int lightMax = 4000;

const int ID = 8;

//Should use constant here that set whether user wants a darker or a brighter environment.
const int lightMin = 3400;

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
 * MAX_MESSAGE_LENGTH_ALLOWED is the maximum length of the message which can be sent and received via UART/NIC
 */
const unsigned int MAX_MESSAGE_LENGTH_ALLOWED = 5;

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
 * Semaphore* semcout;
 */
NIC* g_nic;


/**
 * Functions prototypes to allow they to be declared after the main function.
 */
void configureTheLedsEffects();
void turn_led( int pin, bool on );
int ReceiveCommandUART();
int ReceiveCommandNIC();
int LEDPowerEffect();
void PWMInterrupt();
int RadioThreadFunction();
void InterpretMessage( char msg[ MAX_MESSAGE_LENGTH_ALLOWED ] );

class InterpretCommandMessage : public pj::Listener<pj::Message>
{
public:
	void notify( pj::Message msg )
	{
		char* received;
		received = (char*) msg.getData();
		InterpretMessage(received);
	}
};

/**
 * Main function entry point.
 */
int main()
{
    TSC_Timer pwmTimer( 100, &PWMInterrupt );
    myRadio = new LampRadio;
    myRadioControl = new LampRadioControl( *myRadio, *myRadio, ID );
    InterpretCommandMessage interpreter;
    myRadioControl->addListener(&interpreter);
    configureTheLedsEffects();
    g_nic = new NIC();
    Thread* uartThread;
    Thread* ledEffectThread;
    Thread* radioThread;
    uartThread = new Thread( &ReceiveCommandUART );
    ledEffectThread = new Thread( &LEDPowerEffect );
    radioThread = new Thread( &RadioThreadFunction );
    Alarm::delay( 1e6 );
    int uartThreadStatus = uartThread->join();
    int radioThreadStatus = radioThread->join();
    int ledEffectThreadStatus = ledEffectThread->join();
    return 0;
}


int RadioThreadFunction()
{
	myRadioControl->startListening();
}

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


void InterpretMessage( char msg[ MAX_MESSAGE_LENGTH_ALLOWED ] )
{
    unsigned int led;
    unsigned int pow;
    unsigned int tempDelay;
    unsigned int currentIndex;
    
    switch( msg[ 0 ] )
    {
        case 'R':
        {
            led = 0;
            break;
        }
        case 'G':
        {
            led = 1;
            break;
        }
        case 'B':
        {
            led = 2;
            break;
        }
        case 'A':
        {
            led = MAX_LEDS_ALLOWED_TO_BE_USED;
            break;
        }
        default:
        {
            return;
        }
    }
    
    //led = ((unsigned int)msg[0])-48; // int based on ascii
    if( msg[ 1 ] == 'E' )
    {
        //g_effect
        if( msg[ 2 ] == 'N' ) // turn ON g_effect
        {
            if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // one led
            {         // mutexEffect[led]->unlock();
                g_effect[ led ] = true;
            }
            else   // all leds at once
            {
                for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                {
                    // mutexEffect[currentIndex]->unlock();
                    g_effect[ currentIndex ] = true;
                }
            }
            
            PRINTLN( 1, "Effect[" << led << "]=ON" );
        }
        else if( msg[ 2 ] == 'F' ) // turn OFF g_effect
        {
            if( led < MAX_LEDS_ALLOWED_TO_BE_USED )
            {
                // mutexEffect[led]->lock();
                g_effect[ led ] = false;
            }
            else
            {
                for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                {
                    // mutexEffect[currentIndex]->lock();
                    g_effect[ currentIndex ] = false;
                }
            }
            
        }
        else     // set g_effect delay
        {
            tempDelay     = ( (unsigned int) msg[ 2 ] ) - 48;
            tempDelay    *= 10 ^ ( ( (unsigned int) msg[ 3 ] ) - 48 );
            g_effectDelay = tempDelay;
            
        }
    }
    else if( msg[ 1 ] == '0' || msg[ 1 ] == '1' )
    {
        // fixed power
        pow  = 100 * ( ( (unsigned int) msg[ 1 ] ) - 48 );
        pow += 10 * ( ( (unsigned int) msg[ 2 ] ) - 48 );
        pow += 1 * ( ( (unsigned int) msg[ 3 ] ) - 48 );
        
        if( pow > 100 )
        {
            pow = 100;
        }
        
        if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // only one led
        {
            power[ led ] = pow;
        }
        else   // all leds at once
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
            {
                power[ currentIndex ] = pow;
            }
        }
        
    }
}

void SendMessageToNIC( char msg[ MAX_MESSAGE_LENGTH_ALLOWED ] )
{
    int r;
    pj::Message toSend(-1, ID, 0, msg, MAX_MESSAGE_LENGTH_ALLOWED );
    // != 11 ?
    myRadioControl->sendMessage(toSend);
    
}

int ReceiveCommandUART()
{
    
    unsigned int currentIndex;
    char         msg[ MAX_MESSAGE_LENGTH_ALLOWED ]; //[DATA_SIZE];
    
    UART* uart = new UART();
    
    while( !g_finishThread )
    {
        // messages start with ":"
        do
        {
            msg[ 0 ] = uart->get();
        } while( msg[ 0 ] != ':' );
        
        currentIndex = 0;
        
        while( ( msg[ currentIndex ] != '\n' ) && ( currentIndex < MAX_MESSAGE_LENGTH_ALLOWED ) )
        {
            msg[ currentIndex++ ] = uart->get();
        }
        
        memset( msg + currentIndex, 0x00, MAX_MESSAGE_LENGTH_ALLOWED - currentIndex );
        
        // message received.
        SendMessageToNIC( &msg[ 0 ] );
        InterpretMessage( &msg[ 0 ] );
    }
    
    return 0;
}

int ReceiveCommandNIC()
{
    
    char msg[ MAX_MESSAGE_LENGTH_ALLOWED ];
    
    NIC::Protocol prot;
    NIC::Address  src;
    
    while( !g_finishThread )
    {
        while( !( g_nic->receive( &src, &prot, &msg, sizeof( msg ) ) > 0 ) )
        {
            //PRINTLN( 1, "." );
        }
        
        InterpretMessage( msg );
    }
    
    return 0;
}

int LEDPowerEffect()
{
    
    int          pow;
    unsigned int currentIndex;
    //unsigned int collunmIndex;
    
    //currentIndex = MAX_LEDS_ALLOWED_TO_BE_USED;
    
    while( !g_finishThread )
    {
        //for (currentIndex=0; currentIndex<=MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++) {
        //PRINTLN( 1, "Incresing power of led[" << i << "]" );
        
        for( pow = 0; pow <= 100; pow++ )
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
            {
                // mutexEffect[currentIndex]->lock();
                //if (currentIndex<MAX_LEDS_ALLOWED_TO_BE_USED) // only one led
                
                if( g_effect[ currentIndex ] )
                {
                    power[ currentIndex ] = pow;
                    
                    //else // all leds at once
                    //   for (collunmIndex=0; collunmIndex<MAX_LEDS_ALLOWED_TO_BE_USED; collunmIndex++)
                    //      power[collunmIndex]=pow;
                    // mutexEffect[currentIndex]->unlock();
                    Alarm::delay( g_effectDelay );
                }
            }
        }
        
        for( pow = 100; pow >= 0; pow-- )
        {
            for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
            {
                if( g_effect[ currentIndex ] )
                {
                    // mutexEffect[currentIndex]->lock();
                    //if (currentIndex<MAX_LEDS_ALLOWED_TO_BE_USED)
                    power[ currentIndex ] = pow;
                    //else
                    //   for (collunmIndex=0; collunmIndex<MAX_LEDS_ALLOWED_TO_BE_USED; collunmIndex++)
                    //      power[collunmIndex]=pow;
                    // mutexEffect[currentIndex]->unlock();
                    Alarm::delay( g_effectDelay );
                }
            }
        }
    
    }
    
    // By calling this method, the currently running thread is stopped and put in FINISHING state.
    // If there are "joining threads" for the running thread (i.e., threads that called join() for
    // the running thread), these threads have its state set back to READY and are reinserted into
    // the scheduling queue.
    // static void exit(int status = 0)
    
    return 0;
}

void PWMInterrupt()
{
    static unsigned int dummyCounter = 0;
    static unsigned int checkSensor = 0;
    static int powerToApply [MAX_LEDS_ALLOWED_TO_BE_USED];
    static int leds[] = {10, 9, 11, 23, 8};
    if(!checkSensor)
    {
    	for (unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex)
    	{
    		powerToApply[currentIndex] = DefaultLightVal(power[currentIndex]);
    	}
    }
    
    // not all leds are actually used. Only the RGB ones (the first 3)
    
    for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
    {
        if( dummyCounter < powerToApply[ currentIndex ] )
        {
            turn_led( leds[ currentIndex ], true );
        }
        else
        {
            turn_led( leds[ currentIndex ], false );
        }
    }
    //Check sensor every 3 frames.
    checkSensor = ( checkSensor + 1 ) % 300;
    dummyCounter = ( dummyCounter + 1 ) % 100;
}

/**
 * Stills a mystery what does it do exactly. Maybe to initialize the led effects.
 */
void configureTheLedsEffects()
{
    //Alarm::delay(100);
    
    for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
    {
        //   mutexEffect[currentIndex]= new Mutex();
        //   mutexEffect[currentIndex]->lock(); // g_effect starts OFF (blocked)
        g_effect[ currentIndex ] = false;
    }
}
