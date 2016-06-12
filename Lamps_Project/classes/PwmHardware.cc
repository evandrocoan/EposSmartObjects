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
#include <interfaces/LampControlStrategy.h>



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
    
    /**
     * @see LampControlStrategy::addNewLamp( Lamp, LampConfiguration ) member abstract class declaration.
     */
    void addNewLamp( Lamp* lamp, LampConfiguration* setting )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::addNewLamp(2)" );
    }
    
    /**
     * @see LampControlStrategy::setNewUserSettings( Lamp, LampConfiguration ) member abstract class declaration.
     */
    void setNewUserSettings( Lamp* lamp, LampConfiguration* newSetting )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::setNewUserSettings(2)" );
    }
    
    
private:
    
    /**
     * Creates an PwmHardware object ready to be used by the singleton design pattern.
     */
    PwmHardware()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::PwmHardware(0) THE PRIVATE CONSTRUCTOR!" );
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







