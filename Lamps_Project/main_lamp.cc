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

#include <classes/LampBoard.cc>
#include <classes/Led.cc>
#include <classes/PwmHardware.cc>
#include <classes/LampConfiguration.cc>
#include <classes/Radio.cc>
#include <classes/Usb.cc>
#include <classes/UserBoard.cc>
#include <classes/UserRegistry.cc>

#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>



/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * Main function entry point. To creates and configures the lamp smart object application.
 */
int main()
{
    const char* const PROGRAM_VERSION = "0.1";
    
    PRINTLN( 1, "EposMotesII app MAIN_LAMP initing..." );
    PRINTLN( 1, "Program version: " << PROGRAM_VERSION );
    
    LampConfiguration defaultConfiguration( "blueLed", "nothing", 100 );
    
    LampBoard lampBoard( 1, &defaultConfiguration );
    lampBoard.getLampBoardId();
    
    PRINTLN( 1, "EposMotesII app finishing" );
    return 0;
}










