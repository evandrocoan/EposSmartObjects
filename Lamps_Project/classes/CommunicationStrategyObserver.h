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
#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include <classes/MyClass.h>



/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * This defines an interface for an smart object. Every smart object must to be able to communicates
 * with the other. To accomplish that they also need to use the same communication interface
 * compatible technology as the TCP/IP or the ARM MC13224V built-in 2.4 GHz 802.15.4 RF Radio.
 */
class CommunicationStrategyObserver
{
public:
    
    /**
     * This defines the communication observer notifier function to be used by this project concrete
     * classes.
     * 
     * @see SmartObject interface declaration.
     */
    virtual void receiveMessage( const char* ) = 0;
    
    
protected:
    
    /**
     * Handle all radio communications needed by this project.
     */
    Radio radio;
    
    /**
     * Handle all USB serial communications needed by this project.
     */
    Usb usb;
    
};




