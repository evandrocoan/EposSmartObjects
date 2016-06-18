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



/**
 * This class is the interface to an strategy to control the Lamp's objects.
 */
class PwmHardware : public LampControlStrategy
{
public:
    
    /**
     * Gets the only allowed PwmHardware's instance by lazy initialization.
     * 
     * @return the unique existent PwmHardware's instance.
     */
    static PwmHardware& getInstance()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::getInstance(0)" );
        
        static PwmHardware instance;
        return instance;
    }
    
    
private:
    
    /**
     * Creates an PwmHardware object ready to be used by the singleton design pattern.
     */
    PwmHardware()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::PwmHardware(0) THE PRIVATE CONSTRUCTOR!" );
        
    #if defined DEBUG
        for( int currentIndex = 0; currentIndex < 10; ++currentIndex )
        {
            Alarm::delay( 2e6 );
            
            cout << "\nTHE LIGHT VALUE IS: " << adc.get() << endl;
            cout << "DefaultLightVal  (30): " << DefaultLightVal(30) << endl;
            cout << "Alternative1     (30): " << Alternative1(30) << endl;
            cout << "Alternative2     (30): " << Alternative2(30) << endl;
            cout << "DefaultLightVal  (60): " << DefaultLightVal(60) << endl;
            cout << "Alternative1     (60): " << Alternative1(60) << endl;
            cout << "Alternative2     (60): " << Alternative2(60) << endl;
            cout << "DefaultLightVal (100): " << DefaultLightVal(100) << endl;
            cout << "Alternative1    (100): " << Alternative1(100) << endl;
            cout << "Alternative2    (100): " << Alternative2(100) << endl;
            
            ++currentIndex;
        }
        
        // For initial debugging purpose active the maximum power available.
        for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
        {
            // Comment this when the maximum power is setted by the LampBoard object.
            g_maximum_leds_power[ currentIndex ] = 100;
        }
    #endif
        
        // To creates a interrupt by stealing one Operation System Interrupt. This theft has know side
        // affects until now, but it could be causing the 'traits.h' debug level info, to crash this
        // program while running. The the file at 'file/trace_debug_info_error.txt' on this project
        // for more information about it.
        // The values tested as the interruption time to cause the LEDs to show a human visual blink
        // when the LEDs power are set to lower values as 1, instead of 100 are 250. Note that not all
        // colors showed blinking when using higher values. Some color as Red and Green, just presented
        // themselves with lower bight then they should when the their minimum power is set.
        // When it is set to 100, there are any blink perception.
        DEBUGGERLN( 4, "RUNNING: TSC_Timer pwmTimer( 100, &PWMInterrupt )..." );
        TSC_Timer pwmTimer( 100, &( PwmHardware::PWMInterrupt ) );
        
        //this->lamps = new Vector<Lamp, 10>(); 
        //this->size  = 0;
        
        DEBUGGERLN( 2, "EXITING THE PwmHardware::PwmHardware(0)!" );
    }
    
    /**
     * PWM Interrupt handler. DO NOT USE 'DEBUGGERLN(...)' inside this! If you do it, it will crash the EPOS.
     */
    static void PWMInterrupt()
    {
        static int powerToApply [MAX_LEDS_ALLOWED_TO_BE_USED];
        
        static int          dummyCounter = 0;
        static unsigned int checkSensor  = 0;
        
        // Move this LEDs data into Led objects. Replace this as an class atribute of Led objects
        // dynamic list, added/updated by 'addNewLamp( Lamp*, LampConfiguration* )'.
        static int leds[] = { 10, 9, 11, 23, 8 };
        
    #if defined DEBUG
        // cout << "PWMInterrupt(0)";
        // cout << "dummyCounter: " << dummyCounter << endl;
        static bool firstTime = 0;
        
        if( !( firstTime ) )
        {
            cout << "\nI AM ENTERING ON THE PwmHardware::PWMInterrupt(0)\n";
            cout << "I AM ENTERING ON THE PwmHardware::PWMInterrupt(0)\n";
            cout << "I AM ENTERING ON THE PwmHardware::PWMInterrupt(0)\n" << endl;
            ++firstTime;
        }
        
    #endif
        
        if( !checkSensor )
        {
            for( unsigned int currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; ++currentIndex )
            {
                powerToApply[ currentIndex ] = Alternative2( g_maximum_leds_power[ currentIndex ] );
                
            #if defined DEBUG
                //cout << "powerToApply[" << currentIndex << "]: " << powerToApply[ currentIndex ] << endl;
            #endif
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
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    PwmHardware( PwmHardware const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( PwmHardware const& );
    
    // PwmHardware magic private methods
    
    
    
};







