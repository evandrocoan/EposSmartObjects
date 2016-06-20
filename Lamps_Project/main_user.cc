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


/**
 * DO NOT REMOVE IT! Always move this into the main include linear order.
 * 
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;


#include <utility/ostream.h>
#include <semaphore.h>

#include <cstdarg>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include <headers/software_version.h>

#include <utility/malloc.h>
#include <utility/list.h>
#include <utility/vector.h>
#include <utility/list.h>
#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <traits.h>

#include <classes/UserRegistry.cc>
#include <classes/LampConfiguration.cc>
#include <classes/Message.cc>
#include <interfaces/SmartObjectCommunication.h>
#include <interfaces/CommunicationStrategyObserver.h>
#include <classes/CommunicationSubject.cc>
#include <classes/Usb.cc>
#include <classes/Radio.cc>
#include <classes/Lamp.cc>
#include <interfaces/LampControlStrategy.h>
#include <classes/PwmHardware.cc>
#include <classes/UserBoard.cc>
#include <classes/LampBoard.cc>



/**
 * Main function entry point. To creates and configures the user smart object application.
 */
int main()
{
    PRINTLN( 1, "EposMotesII app MAIN_USER initing..." );
    PRINTLN( 1, "Program Version: " << PROGRAM_VERSION << ", Id: " << EPOS_MOSTES_BOARD_ID );
    
    PRINTLN( 1, "To send commands to the EPOSMotes2 by USB device, " ); 
    PRINTLN( 1, "to others EPOSMotes2 devices listening by Radio, use: " );
    PRINTLN( 1, "echo :R100 > /dev/ttyUSB0" );
    PRINTLN( 1, "Try also :REN, :BEN, :GEN or :AEN" ); 
    
    UserBoard userBoard( EPOS_MOSTES_BOARD_ID );
    CommunicationSubject::getInstance().addObserver( &userBoard );
    
    userBoard.addUserConfiguration( new LampConfiguration( "BlueLed", "light_sensor", 100 ) );
    //userBoard.addUserConfiguration( new LampConfiguration( "RedLed", "light_sensor", 100 ) );
    //userBoard.addUserConfiguration( new LampConfiguration( "GreenLed", "light_sensor", 100 ) );
    
    // DO NOT USE 'cout' ON FUNCTIONS WHICH RETURN 'const types'! First save it into a variable,
    // then ONLY THEN USE 'cout' ON IT! If you do it, it will crash the EPOS.
    DEBUGGERLN( 1, "RUNNING: const int lampBoardId = userBoard.getLampId();");
    const int userBoardId = userBoard.getUserId();
    
    DEBUGGERLN( 1, "userBoard.getLampId(): " << userBoardId );
    userBoard.waitForCommunications();
    
    PRINTLN( 1, "EposMotesII app finishing" );
    return 0;
}










