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
 * It notifies the LampControlObserver's objects when the subject is changed/modified.
 */
class LampControlSubject
{
public:
    
    /**
     * Gets the only allowed LampControlSubject's instance by lazy initialization.
     * 
     * @return the unique existent LampControlSubject's instance.
     */
    static LampControlSubject& getInstance();
    
    /**
     * Adds a new LampControlObserver observer to be notified by changes on the observed subject.
     * 
     * @param observer        the LampControlObserver observer.
     * @return true when successfully add the observer, false otherwise.
     */
    bool addObserver( LampControlObserver* );
    
    /**
     * This calls the observer and to send its message.
     * 
     * @param message      the message to send to the observer
     * @see SmartObjectCommunication::receiveMessage( const char* ) member class declaration for the message protocol.
     */
    void notifyObservers( const char* );
    
    
private:
    
    /**
     * Contains the observers pointers.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< LampControlObserver* > observers;
    
    /**
     * Creates an Subject object ready to be used by the singleton design pattern.
     */
    LampControlSubject();
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    LampControlSubject( LampControlSubject const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( LampControlSubject const& );
    
};



/**
 * @see LampControlSubject::getInstance() member class declaration.
 */
static LampControlSubject& LampControlSubject::getInstance()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE LampControlSubject::getInstance(0)" );
    
    static LampControlSubject instance;
    return &instance;
}

/**
 * @see LampControlSubject::LampControlSubject() member class declaration.
 */
LampControlSubject::LampControlSubject() : isTheObserverAdded( false )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE LampControlSubject::LampControlSubject(0) CONSTRUCTOR!" );
}

/**
 * @see LampControlSubject::LampControlSubject( LampControlObserver* ) member class declaration.
 */
bool LampControlSubject::addObserver( LampControlObserver* observer )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE LampControlSubject::addObserver(1)" );
    Ordered_List< LampControlObserver* >::Element* element = new Ordered_List< LampControlObserver* >::Element( observer );
    
    observers.insert( element );
    return true;
}

/**
 * @see LampControlSubject::notifyObservers( const char* ) member class declaration.
 */
bool LampControlSubject::notifyObservers()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE LampControlSubject::notifyObservers(1)" );
    
    if( this->observers.size() )
    {
        auto endIterator = observers.end();
        
        for( Ordered_List< LampControlObserver* >::Iterator iterator = observers.begin();
             iterator != endIterator; ++iterator )
        {
            cout << ( *iterator )->object()->applyLightConfiguration();
        }
        
        return true;
    }
    
    return false;
}

