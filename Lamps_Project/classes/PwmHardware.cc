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


#include <interfaces/LampControlStrategy.h>



/**
 * Preprocessor directive designed to cause the current source file to be included only once in a
 * single compilation. Thus, serves the same purpose as #include guards, but with several
 * advantages, including: less code, avoidance of name clashes, and sometimes improvement in
 * compilation speed. In main file this is enabled by default.
 */
#pragma once


/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * This class is the interface to an strategy to control the Lamp's objects.
 */
class PwmHardware : private LampControlStrategy
{
public:
    
    /**
     * Gets the only allowed PwmHardware's instance by lazy initialization.
     * 
     * @return the unique existent PwmHardware's instance.
     */
    static PwmHardware& getInstance();
    
    /**
     * @see LampControlStrategy::addNewLamp( Lamp, LampConfigurationStrategy ) member abstract class declaration.
     */
    void addNewLamp( lamp Lamp, setting LampConfigurationStrategy ) = 0;
    
    /**
     * @see LampControlStrategy::setNewUserSettings( Lamp, LampConfigurationStrategy ) member abstract class declaration.
     */
    void setNewUserSettings( lamp Lamp, newSetting LampConfigurationStrategy ) = 0;
    
    
private:
    
    /**
     * Creates an PwmHardware object ready to be used by the singleton design pattern.
     */
    PwmHardware();
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    PwmHardware( PwmHardware const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( PwmHardware const& );
    
    // PwmHardware magic methods
    
};



/**
 * @see PwmHardware::getInstance() member class declaration.
 */
static PwmHardware& PwmHardware::getInstance()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::getInstance(0)" );
    
    static PwmHardware instance;
    return &instance;
}

/**
 * @see PwmHardware::PwmHardware() member class declaration.
 */
PwmHardware::PwmHardware()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE PwmHardware::PwmHardware(0) THE PRIVATE CONSTRUCTOR!" );
}




