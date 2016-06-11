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
 * This class implementes the Lamp's objects using the LampConfiguration abstract class.
 */
class LampConfiguration
{
public:
    
    /**
     * 
     */
    LampConfiguration()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::LampConfiguration(0) THE CONSTRUCTOR!" );
    }
    
    /**
     * @see LampConfiguration::getBright() member abstract class declaration.
     */
    int getBright()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::getBright(0)" );
        
        return 0; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * @see LampConfiguration::getCurrentBright() member abstract class declaration.
     */
    int getCurrentBright()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::getCurrentBright(0)" );
        
        return 0; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * @see LampConfiguration::setBright(int) member abstract class declaration.
     */
    bool setBright( int brigth )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::setBright(1) | brigth: " << brigth );
        
        return false; // Just like a Guto's EPOS implementation.
    }
    
    
private:
    
    /**
     * 
     */
    int maximumBright;
    
    /**
     * 
     */
    const char* lampType;
    
    
};











