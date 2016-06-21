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



/**
 * MAX_MESSAGE_LENGTH_ALLOWED is the maximum length of the message which can be sent and received via UART/NIC
 */
const unsigned int MAX_MESSAGE_LENGTH_ALLOWED = 5;



/**
 * This class implements the EPOSMotes 2 serial USB use. The Startup Board is equipped with and UART
 * to USB transceiver. The UART bus is connected directly to FT232RL chip for a PC interface with
 * the Processing Module, discarding the need of a proper USB stack.
 * 
 * When the USB receives a message it should notify its observer by calling its inherited subject
 * object subject.
 * 
 * @see CommunicationSubject::notifyObserver( const char* ) member class declaration for the subject
 *      object use.
 */
class Usb
{
public:
    
    /**
     * Gets the only allowed Usb's instance by lazy initialization.
     * 
     * @return the unique existent Usb's instance.
     */
    static Usb& getInstance()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Usb::getInstance(0)" );
        static Usb instance;
        
        if( !Usb::isThisObjectCreated )
        {
            Usb::thisObject          = &instance;
            Usb::usbUartController   = new UART();
            Usb::subject             = &CommunicationSubject::getInstance();
            Usb::uartThread          = new Thread( &uartReceiver );
            Usb::isThisObjectCreated = true;
        }
        
        return instance;
    }
    
    
private:
    
    /**
     * The EPOS UART controller.
     * 
     * @see <http://epos.lisha.ufsc.br/EPOS+User+Guide#UART>
     */
    static UART* usbUartController;
    
    /**
     * This is the thread responsible for pooling the USB board to receive new incoming messages
     * and is responsible to notify the subject if receives any message.
     */
    static Thread* uartThread;
    
    /**
     * This static variable must to be setted on this objects creation. This is to allow the object
     * access within the static context.
     */
    static Usb* thisObject;
    
    /**
     * Used to notify the CommunicationStrategyObserver's observes when a message is received.
     */
    static CommunicationSubject* subject;
    
    /**
     * Indicates whether this object is already initialized.
     */
    static bool isThisObjectCreated;
    
    /**
     * Creates an USB object ready to be used by the singleton design pattern.
     */
    Usb()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Usb::Usb(0) THE PRIVATE CONSTRUCTOR!" );
    }
    
    /**
     * Runs the thread which calls the subject.
     * 
     * @see Usb::uartThread class attribute declaration.
     */
    static int uartReceiver()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Usb::uartReceiver(0)" );
        PRINTLN( 1, "Thread UART initing..." );
        
        char msg[ MAX_MESSAGE_LENGTH_ALLOWED ]; //[DATA_SIZE];
        
        while( true )
        {
            // messages start with ":"
            do
            {
                DEBUGGERLN( 1, " ( uartReceiver ) | Messages start with ':'" );
                msg[ 0 ] = Usb::usbUartController->get();
                
            } while( msg[ 0 ] != ':' );
            
            DEBUGGERLN( 1, "RUNNING: currentIndex = 0" );
            short currentIndex = 1;
            
        #if defined DEBUG
            cout << "RUNNING: while( msg[" << currentIndex << "] != '\\n' )" << endl;
            cout << "RUNNING: while( msg[" << currentIndex << "] != '\\n' )" << endl;
            cout << "RUNNING: while( msg[" << currentIndex << "] != '\\n' )" << endl;
        #endif
            
            while( ( msg[ currentIndex ] != '\n' )
                     && ( currentIndex < MAX_MESSAGE_LENGTH_ALLOWED ) )
            {
                
                DEBUGGERLN( 1, "RUNNING: msg[" << currentIndex << "] = Usb::usbUartController->get();" );
                msg[ currentIndex ] = Usb::usbUartController->get();
                
                DEBUGGERLN( 1, " ( uartReceiver ) | msg[" << currentIndex << "] = " << msg[ currentIndex ] );
                ++currentIndex;
            }
            
            DEBUGGERLN( 1, "RUNNING: memset(3)" );
            memset( msg + currentIndex, 0x00, MAX_MESSAGE_LENGTH_ALLOWED - currentIndex );
            
            // message received.
            DEBUGGERLN( 1, " ( uartReceiver ) | USB/UART Message received: " << &msg[ 0 ] );
            subject->notifyObserver( &msg[ 1 ] );
        }
        
        PRINTLN( 1, " ( uartReceiver ) | Thread UART finishing..." );
        return 0;
    }
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    Usb( Usb const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( Usb const& );
    
    // USB magic methods
    
};



/**
 * To initialize the static class variable.
 */
bool                  Usb::isThisObjectCreated = false;
Usb*                  Usb::thisObject          = NULL;
CommunicationSubject* Usb::subject             = NULL;
Thread*               Usb::uartThread          = NULL;
UART*                 Usb::usbUartController   = NULL;






