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


#include <classes/SmartObject.h>
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
 * This is an abstract class which aggregates to the SmartObject interface the observer design
 * pattern subject object. It inherits the SmartObject interface and is meant to be used within
 * the EPOSMotes2 board.
 */
class EposMotes2Communication : private SmartObject
{
public:
    
    /**
     * @see SmartObject::sendMessage( const char* ) member class declaration.
     */
    virtual void sendMessage( const char* ) = 0;
    
    /**
     * @see SmartObject::receiveMessage( const char* ) member class declaration.
     */
    virtual void receiveMessage( const char* ) = 0;
    
    
protected:
    
    /**
     * The communication subject to be used to call the observers.
     */
    CommunicationSubject subject;
    
};




