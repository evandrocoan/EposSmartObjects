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


#include <interfaces/LampConfigurationStrategy.h>



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
 * This class implementes the Lamp's objects using the LampConfigurationStrategy abstract class.
 */
class LightSensor : private LampConfigurationStrategy
{
public:
    
    /**
     * 
     */
    LightSensor();
    
    /**
     * @see LampConfigurationStrategy::getBright() member abstract class declaration.
     */
    int getBright();
    
    /**
     * @see LampConfigurationStrategy::getCurrentBright() member abstract class declaration.
     */
    int getCurrentBright();
    
    /**
     * @see LampConfigurationStrategy::setBright(int) member abstract class declaration.
     */
    bool setBright( int );
    
    
private:
    
    
    
};



/**
 * @see LightSensor::LightSensor() member class declaration.
 */
LightSensor::LightSensor()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE LightSensor::LightSensor(0) THE CONSTRUCTOR!" );
}








