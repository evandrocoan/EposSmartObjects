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
 * DO NOT MODIFIY IT!
 * 
 * @see The Class LampBoard main include declaration/explanation.
 */
#include <interfaces/CommunicationStrategyObserver.h>



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
    static CommunicationSubject& getInstance()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::getInstance(0)" );
        
        if( !CommunicationSubject::isThisObjectCreated )
        {
            CommunicationSubject::isThisObjectCreated = true;
            CommunicationSubject::thisObject          = new CommunicationSubject();
        }
        
        return *( CommunicationSubject::thisObject );
    }
    
    /**
     * Adds the only one observer which will be allowed to observe the CommunicationStrategyObserver.
     * 
     * @param observer        the CommunicationStrategyObserver observer.
     * @return true when successfully add the observer, false otherwise.
     * 
     * @note only one observer can be added on this implementation.
     * @see CommunicationSubject class declaration/doc.
     */
    bool addObserver( CommunicationStrategyObserver* observer )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::addObserver(1)" );
        
        if( !this->isTheObserverAdded )
        {
            this->observer           = observer;
            this->isTheObserverAdded = true;
            
            DEBUGGERLN( 1, "    ( addObserver ) Returning true." );
            return true;
        }
        
        DEBUGGERLN( 1, "    ( addObserver ) Returning false." );
        return false;
    }
    
    /**
     * This calls the observer and to send its message.
     * 
     * @param config      the configuration to send to the observer
     * @see SmartObjectCommunication::receiveMessage( const char* ) member class declaration for the message protocol.
     */
    bool notifyObserver( LampConfiguration* config )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::notifyObserver(1)" );
        
        Message message( "Config", config );
        return this->notifyObserver( message );
    }
    
    /**
     * This calls the observer and to send its message.
     * 
     * @param textMessage      the text message to send to the observer
     * @see SmartObjectCommunication::receiveMessage( const char* ) member class declaration for the message protocol.
     */
    bool notifyObserver( char* textMessage )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::notifyObserver(1)" );
        
        Message message( const_cast<char *>( textMessage ), NULL );
        return this->notifyObserver( message );
    }
    
    /**
     * This calls the observer and to send its message.
     * 
     * @param message      the Message class object to send to the observer
     * @see SmartObjectCommunication::receiveMessage( const char* ) member class declaration for the message protocol.
     */
    bool notifyObserver( Message message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::notifyObserver(1)" );
        
        if( this->isTheObserverAdded )
        {
            this->observer->receiveMessage( message );
            
            DEBUGGERLN( 1, "    ( notifyObserver ) Returning true." );
            return true;
        }
        
        DEBUGGERLN( 1, "    ( notifyObserver ) Returning false." );
        return false;
    }
    
    
private:
    
    /**
     * This is the only observer allowed.
     * 
     * @see CommunicationSubject class declaration.
     */
    CommunicationStrategyObserver* observer;
    
    /**
     * This static variable must to be setted on this objects creation. This is to allow the object
     * access within the static context.
     */
    static CommunicationSubject* thisObject;
    
    /**
     * Indicates whether the observer is added or not.
     */
    bool isTheObserverAdded;
    
    /**
     * Indicates whether this object is already initialized.
     */
    static bool isThisObjectCreated;
    
    /**
     * Creates an Subject object ready to be used by the singleton design pattern.
     */
    CommunicationSubject() :
            isTheObserverAdded( false )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE CommunicationSubject::CommunicationSubject(0) THE PRIVATE CONSTRUCTOR!" );
    }
    
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
 * To initialize the static class variable.
 */
bool                   CommunicationSubject::isThisObjectCreated = false;
CommunicationSubject*  CommunicationSubject::thisObject          = NULL;








