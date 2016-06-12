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
#include <classes/PwmHardware.cc>



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
class Radio : public SmartObjectCommunication
{
public:
    
    /**
     * Gets the only allowed Radio's instance by lazy initialization.
     * 
     * @return the unique existent Radio's instance.
     */
    static Radio& getInstance( const unsigned char sink_id )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::getInstance(0)" );
        
        static Radio instance;
        
        if( !isThisObjectCreated )
        {
            Radio::sink_id             = sink_id;
            Radio::thisObject          = &instance;
            Radio::subject             = &CommunicationSubject::getInstance();
            Radio::nicThread           = new Thread( &receiver );
            Radio::isThisObjectCreated = true;
        }
        
        return instance;
    }
    
    /**
     * @see SmartObjectCommunication::sendMessage( const char* ) member class declaration.
     */
    void sendMessage( Message message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::sendMessage(1) | message: \n" << message.message );
        
        int currentIndex;
        
        for( currentIndex = 1; currentIndex < 8; currentIndex++ )
        {
            nic.send( sink_id, 0, &message, sizeof( message ) );
            Alarm::delay( 100000 );
        }
    }
    
    
private:
    
    /**
     * The EPOS radio controller.
     * 
     * @see <http://epos.lisha.ufsc.br/EPOS+User+Guide#NIC>
     */
    static NIC nic;
    
    /**
     * This is the thread responsible for pooling the radio board to receive new incoming messages
     * and is responsible to notify the subject if receives any message.
     */
    static Thread* nicThread;
    
    /**
     * This object be allowed access the the thread, because it cannot receive parameters. This
     * static variable must to be setted on this objects creation.
     */
    static Radio* thisObject;
    
    /**
     * Used to notify the CommunicationStrategyObserver's observes when a message is received.
     */
    static CommunicationSubject* subject;
    
    /**
     * Indicates whether this object is already initialized.
     */
    static bool isThisObjectCreated;
    
    /**
     * 
     */
    static unsigned char sink_id;
    
    /**
     * Creates an radio object ready to be used by the singleton design pattern.
     */
    Radio()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::Radio(0) THE PRIVATE CONSTRUCTOR!" );
    }
    
    /**
     * Runs the thread which calls the subject.
     * 
     * @see Radio::nicThread class attribute declaration.
     */
    static int receiver()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Radio::receiver(0)" );
        
        NIC::Protocol prot;
        NIC::Address  src;
        
        LampConfiguration config( "unused", "unused", 50 );
        Message message( "Empty", config );
        
        DEBUGGERLN( 1, "\nReceiver" );

        while( true )
        {
            while ( !( nic.receive( &src, &prot, &message, sizeof( message ) ) > 0 ) );
            
            DEBUGGERLN( 1, "####################\n" );
            DEBUGGERLN( 1, "Configured brightness: " << message.config.getBright() << "\n" );
            DEBUGGERLN( 1, "Lamp type: " << message.config.lampType << "\n" );
            DEBUGGERLN( 1, "Flags: " << message.config.specialFlags << "\n" );
            
            thisObject->subject->notifyObserver( message );
        }
        
        return 0;
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


bool Radio::isThisObjectCreated = false;







