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
 * This is the subject which is used the the observed object to call/notify its observer to
 * perform their operations.
 * 
 * On this implementation there is only one observer allowed to due this also implements the
 * Strategy Design Pattern, then only one strategy can be allowed to be used at a time.
 */
class CommunicationSubject
{
public:
    
    /**
     * Gets the only allowed CommunicationSubject's instance by lazy initialization.
     * 
     * @return the unique existent CommunicationSubject's instance.
     */
    static CommunicationSubject& getInstance();
    
    /**
     * Adds the only one observer allowed which will be allowed to observe the EposMotes2Communication.
     * 
     * @param observer        the EposMotes2Communication observer.
     * @return true when successfully add the observer, false otherwise.
     * 
     * @note only one observer can be added on this implementation.
     * @see CommunicationSubject class declaration.
     */
    bool addObserver( CommunicationStrategyObserver* );
    
    /**
     * This calls the observer and to send its message.
     * 
     * @param message      the message to send to the observer
     * @see SmartObject::receiveMessage( const char* ) member class declaration for the message protocol.
     */
    void notifyObserver( const char* );
    
    
private:
    
    /**
     * This is the only observer allowed.
     * 
     * @see CommunicationSubject class declaration.
     */
    CommunicationStrategyObserver* observer;
    
    /**
     * Indicates whether the observer is added or not.
     */
    bool isTheObserverAdded;
    
    /**
     * Creates an Subject object ready to be used by the singleton design pattern.
     */
    CommunicationSubject();
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    CommunicationSubject( CommunicationSubject const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( CommunicationSubject const& );
    
};



/**
 * @see CommunicationSubject::getInstance() member class declaration.
 */
static CommunicationSubject& CommunicationSubject::getInstance()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::getInstance(0)" );
    
    static CommunicationSubject INSTANCE;
    return &INSTANCE;
}

/**
 * @see CommunicationSubject::CommunicationSubject() member class declaration.
 */
CommunicationSubject::CommunicationSubject()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::CommunicationSubject(0) CONSTRUCTOR!" );
    
    isTheObserverAdded = false;
}

/**
 * @see CommunicationSubject::CommunicationSubject( CommunicationStrategyObserver* ) member class declaration.
 */
bool CommunicationSubject::addObserver( CommunicationStrategyObserver* observer )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::addObserver(1)" );
    
    if( !isTheObserverAdded )
    {
        this->observer     = observer;
        isTheObserverAdded = true;
        
        return true;
    }
    
    return false;
}

/**
 * @see CommunicationSubject::notifyObserver( const char* ) member class declaration.
 */
bool CommunicationSubject::notifyObserver( const char* message )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::notifyObserver(1)" );
    
    if( isTheObserverAdded )
    {
        this->observer->receiveMessage( message );
        return true;
    }
    
    return false;
}

