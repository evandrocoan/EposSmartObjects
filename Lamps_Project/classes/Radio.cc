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
#include <interfaces/SmartObjectCommunication.h>



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
 * This class implements the EPOSMotes 2 radio use. It radio is an ARM MC13224V built-in 2.4 GHz
 * 802.15.4 RF Radio.
 * 
 * When the radio receives a message it should notify its observer by calling its inherited subject
 * object subject.
 * 
 * @see CommunicationSubject::notifyObserver( const char* ) member class declaration for the subject
 *      object use.
 */
class Radio : private SmartObjectCommunication
{
public:

    /**
     * Gets the only allowed Radio's instance by lazy initialization.
     * 
     * @return the unique existent Radio's instance.
     */
    static Radio& getInstance()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::getInstance(0)" );
        
        static Radio instance;
        return &instance;
    }
    
    /**
     * @see SmartObjectCommunication::sendMessage( const char* ) member class declaration.
     */
    void sendMessage( const char* message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::sendMessage(1) | message: \n" << message );
    }
    
    
private:
    
    /**
     * Creates an radio object ready to be used by the singleton design pattern.
     */
    Radio()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::Radio(0) THE PRIVATE CONSTRUCTOR!" );
    }
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    Radio( Radio const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( Radio const& );
    
    // radio magic methods
    
    
    
};








