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
 * DO NOT MODIFIY IT!
 * 
 * @see The Class LampBoard main include declaration/explanation.
 */
#include <classes/PwmHardware.cc>



/**
 * This class implements Led lamps using the abstract class Lamp.
 */
class Led : public Lamp
{
public:
    
    /**
     * 
     */
    Led()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::Led(0) THE CONSTRUCTOR!" );
    }
    
    /**
     * @see Lamp::setNewUserSetting( LampConfiguration ) member abstract class declaration.
     */
    void setNewUserSetting( LampConfiguration* setting )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::setNewUserSetting(1)" );
    }
    
    /**
     * @see Lamp::getCurrentBright() member abstract class declaration.
     */
    int getCurrentBright()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::getCurrentBright()" );
        
        return 0; return 0; // Just like a Guto's EPOS implementation.
    }
    
    
private:
    
    // private magic stuff
    
    
    
};









