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
 * This class implements the interface used to allow the observers to be called when the observed
 * object is changed.
 * 
 * This class is meant to represent a lamp board.
 * 
 * @see CommunicationStrategyObserver abstract class declaration.
 */
class LampBoard : public CommunicationStrategyObserver
{
public:
    
    /**
     * To creates a new lampBoard object and sets this board ID and priority.
     * 
     * @param boardId                 the current board user ID as an integer number.
     * @param configuration           the LampConfiguration default lamp configuration object.
     */
    LampBoard( const int boardId ) :
           lampBoardId( boardId ),
           defaultConfigurations(),
           //controlStrategies(),
           priorityUsers()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::LampBoard(2) CONSTRUCTOR!" );
        
        this->usb             = &Usb::getInstance();
        this->radio           = &Radio::getInstance( boardId );
        this->controlStrategy = &( PwmHardware::getInstance() );
    }
    
    /**
     * @see CommunicationStrategyObserver::receiveMessage abstract member class declaration.
     */
    void receiveMessage( Message message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::receiveMessage(1) | message: " << message.message );
        this->InterpretMessage( message.message );
    }
    
    /**
     * Add a new user's priority to this lamp.
     * 
     * @param userPriority     the priority user's level as an integer number.
     * @param userId           the priority user's identification number as an integer number.
     * 
     * @return true when the priority user is successfully added, otherwise false.
     */
    bool addPriorityUser( const int userPriority, const int userId )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::addPriorityUser(2) | userPriority: "
                << userPriority
                << "userId: " << userId );

        UserRegistry* user = new UserRegistry(userPriority, userId);
        Ordered_List< UserRegistry* >::Element* element = new Ordered_List< UserRegistry* >::Element(&user);

        priorityUsers.insert(element);
        
        return true;
    }
    
    /**
     * Gets the current board unique board identification number.
     * 
     * @return this lamp board identification as a const integer.
     */
    const int getLampId()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::getLampId(0)" );
        return this->lampBoardId;
    }
    
    /**
     * @see Radio::waitForCommunications() member class declaration.
     */
    void waitForCommunications()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::waitForCommunications(0)" );
        this->radio->waitForCommunications();
    }
    
    /**
     * When adding a new lamp, saves its first/default configuration to be restaured later.
     * 
     * @see LampControlStrategy::addNewLamp() member class declaration.
     */
    bool addNewLamp( Lamp* lamp )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::addNewLamp(1)" );
        
        // TODO: To save this lamp index, to find out it later to restore the configuration.
        //defaultConfigurations.insert( lamp->getUserSetting() );
        
        return this->controlStrategy->addNewLamp( lamp );
    }
    
    /**
     * @see LampControlStrategy::showLampSettingsAndIndex() member class declaration.
     */
    void showLampSettingsAndIndex()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::showLampSettingsAndIndex(0)" );
        this->controlStrategy->showLampSettingsAndIndex();
    }
    
    
private:
    
    /**
     * An unique integer representing this lamp identification number.
     */
    const int lampBoardId;
    
    /**
     * Handle all radio communications needed by this project.
     */
    Radio* radio;
    
    /**
     * Handle all USB serial communications needed by this project.
     */
    Usb* usb;
    
    /**
     * The default lamp configuration used the there is no user around.
     */
    Ordered_List< LampConfiguration* > defaultConfigurations;
    
    /**
     * Contains the control strategies this board is using.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    // Let it out for simplification and time saving, so just use one strategy, the PwmHardware.
    //Ordered_List< LampControlStrategy* > controlStrategies;
    //temp for testing
    LampControlStrategy* controlStrategy;
    
    /**
     * Contains the user's which have priority over non-registered users.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< UserRegistry* > priorityUsers;
    
    /**
     * 
     * 
     * @param message       the message to interpret.           
     */
    void InterpretMessage( const char message[ MAX_MESSAGE_LENGTH_ALLOWED ] )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::InterpretMessage(1) | message: " << message );
        
        unsigned int led;
        unsigned int pow;
        //unsigned int tempDelay;
        unsigned int currentIndex;
        
        switch( message[ 0 ] )
        {
            case 'R':
            {
                led = 0;
                break;
            }
            case 'G':
            {
                led = 1;
                break;
            }
            case 'B':
            {
                led = 2;
                break;
            }
            case 'A':
            {
                led = MAX_LEDS_ALLOWED_TO_BE_USED;
                break;
            }
            default:
            {
                PRINTLN( 1, "Invalid led value '" << message[ 0 ] << "' on position 0" );
                return;
            }
        }
        
        //led = ((unsigned int)message[0])-48; // int based on ascii
        /*if( message[ 1 ] == 'E' )
        {
            //g_effect
            if( message[ 2 ] == 'N' ) // turn ON g_effect
            {
                if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // one led
                {         // mutexEffect[led]->unlock();
                    g_effect[ led ] = true;
                }
                else   // all leds at once
                {
                    for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                    {
                        // mutexEffect[currentIndex]->unlock();
                        g_effect[ currentIndex ] = true;
                    }
                }
                
                PRINTLN( 1, "Effect[" << led << "]=ON" );
            }
            else if( message[ 2 ] == 'F' ) // turn OFF g_effect
            {
                if( led < MAX_LEDS_ALLOWED_TO_BE_USED )
                {
                    // mutexEffect[led]->lock();
                    g_effect[ led ] = false;
                }
                else
                {
                    for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                    {
                        // mutexEffect[currentIndex]->lock();
                        g_effect[ currentIndex ] = false;
                    }
                }
                
                PRINTLN( 1, "Effect[" << led << "]=OFF" );
            }
            else     // set g_effect delay
            {
                tempDelay     = ( (unsigned int) message[ 2 ] ) - 48;
                tempDelay    *= 10 ^ ( ( (unsigned int) message[ 3 ] ) - 48 );
                g_effectDelay = tempDelay;
                
                PRINTLN( 1, "Delay=" << g_effectDelay );
            }
        }*/
        
        if( message[ 1 ] == '0' || message[ 1 ] == '1' )
        {
            // fixed power
            pow  = 100 * ( ( (unsigned int) message[ 1 ] ) - 48 );
            pow += 10 * ( ( (unsigned int) message[ 2 ] ) - 48 );
            pow += 1 * ( ( (unsigned int) message[ 3 ] ) - 48 );
            
            if( pow > 100 )
            {
                pow = 100;
            }
            
            if( led < MAX_LEDS_ALLOWED_TO_BE_USED ) // only one led
            {
                // TODO delegate it to PwmHardware power[]
                //power[ led ] = pow;
            }
            else   // all leds at once
            {
                for( currentIndex = 0; currentIndex < MAX_LEDS_ALLOWED_TO_BE_USED; currentIndex++ )
                {
                    // TODO delegate it to PwmHardware power[]
                    //power[ currentIndex ] = pow;
                }
            }
            
            PRINTLN( 1, "Power[" << led << "]=" << pow );
        }
        else
        {
            PRINTLN( 1, "Invalid value '" << message[ 1 ] << "' on position 1" );
        }
    }
    
};







