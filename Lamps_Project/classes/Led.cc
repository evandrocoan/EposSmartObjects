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


#include <classes/Radio.cc>
#include <classes/Usb.cc>



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
 * This class implements Led lamps using the abstract class Lamp.
 */
class Led : private Lamp
{
public:
    
    /**
     * 
     */
    Led();
    
    /**
     * @see Lamp::setNewUserSetting( LampConfigurationStrategy ) member abstract class declaration.
     */
    void setNewUserSetting( setting LampConfigurationStrategy );
    
    /**
     * @see Lamp::getCurrentBright() member abstract class declaration.
     */
    int getCurrentBright();
    
    
private:
    
    
    
};



/**
 * @see Led::Led() member class declaration.
 */
Led::Led()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE Led::Led(0) THE CONSTRUCTOR!" );
}





