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
 * This class is the interface to an strategy to hold the the Lamp's objects configuration.
 */
class LampConfigurationStrategy
{
public:
    
    /**
     * Gets the maximum bright desired to the lamp achieve.
     * 
     * @return an integer as the desired bright to the lamp. This is changed by setBright(int).
     */
    virtual int getBright() = 0;
    
    /**
     * Gets the current bright to the lamp achieve.
     * 
     * @return an integer as current lamp bright to be show. This is changed by some variable as
     *         the current environment temperature.
     */
    virtual int getCurrentBright() = 0;
    
    /**
     * Adjust the maximum bright desired to the lamp achieve.
     * 
     * @param newBright              an integer as the new desired bright to the lamp.
     */
    virtual bool setBright( int ) = 0;
    
    
protected:
    
    /**
     * The current maximum bright desired to the lamp achieve.
     */
    int maximumBright;
    
};




