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
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * This class implements the EPOSMotes 2 serial USB use. The Startup Board is equipped with and UART
 * to USB transceiver. The UART bus is connected directly to FT232RL chip for a PC interface with
 * the Processing Module, discarding the need of a proper USB stack.
 * 
 * When the USB receives a message it should notify its observer by calling its inherited subject
 * object subject.
 * 
 * @see CommunicationSubject::notifyObserver( const char* ) member class declaration for the subject
 *      object use.
 */
class Usb
{
public:
    
    /**
     * Gets the only allowed Usb's instance by lazy initialization.
     * 
     * @return the unique existent Usb's instance.
     */
    static Usb& getInstance()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Usb::getInstance(0)" );
        
        static Usb instance;
        return instance;
    }

    
private:
    
    /**
     * Creates an USB object ready to be used by the singleton design pattern.
     */
    Usb()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Usb::Usb(0) THE PRIVATE CONSTRUCTOR!" );
    }
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    Usb( Usb const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( Usb const& );
    
    // USB magic methods
    
};









