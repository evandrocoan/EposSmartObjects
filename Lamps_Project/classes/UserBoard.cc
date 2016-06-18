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
 * This class is meant to represent a user board.
 * 
 * @see CommunicationStrategyObserver abstract class declaration.
 */
class UserBoard : public CommunicationStrategyObserver
{
public:
    
    /**
     * To creates a new userBoard object and sets this board ID and priority.
     * 
     * @param boardId             the current board user ID as an integer number.
     */
    UserBoard( const int boardId ) :
               userBoardId( boardId ),
               configurations()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::UserBoard(2) CONSTRUCTOR! | boardId: "
                << boardId );
        
        this->usb             = &Usb::getInstance();
        this->radio           = &Radio::getInstance( boardId );
    }
    
    /**
     * @see CommunicationStrategyObserver::receiveMessage abstract member class declaration.
     */
    void receiveMessage( Message message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::receiveMessage(1) | message: \n" << message.message );
    }
    
    /**
     * Gets the current board unique user identification number.
     * 
     * @return this user board identification as a const integer.
     */
    const int getUserId()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::getUserId() | userBoardId: " << this->userBoardId );
        return this->userBoardId;
    }
    
    /**
     * Add a new user's configuration to this user's board.
     * 
     * @param configuration          the user's new configuration to add.
     * @return true when the user configuration is successfully added, otherwise false.
     */
    bool addUserConfiguration( LampConfiguration* configuration )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::addUserConfiguration(1)");
        return false; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * Gets a specified user's configuration by an index.
     * 
     * @param index           the user's configuration index as an integer value.
     * @return the specified user's configuration. NULL when an invalid index is provided.
     */
    LampConfiguration* getUserConfiguration( int index )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::getUserConfiguration(1) | index: " << index );
        return NULL; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * Changes a specified user's configuration to be passed to the LampBoard as soon as the user
     * approach one lamp device.
     * 
     * @param index           the user's configuration index as an integer value.
     * @param config          the new configuration to replace the existent one.
     * @return true when the specified user's configuration is set, otherwise false.
     */
    bool setUserConfiguration( int index, LampConfiguration* config )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::setUserConfiguration(2) | index: " << index );
        return false; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * Print to the screen all the users created configurations.
     */
    void showLampSettingsAndIndex()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::showLampSettingsAndIndex(0)" );
        
        /*int currentIndex  = 0;
        int numberOfLamps = this->configurations->tamanho();
        
        while( currentIndex < numberOfLamps )
        {
            DEBUGGERLN( 1, "Lamp(" << currentIndex << ")" << this->configurations->mostra( currentIndex )->getLampType() );
            
            ++currentIndex;
        }*/
    }
    
    /**
     * @see Radio::waitForCommunications() member class declaration.
     */
    void waitForCommunications()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::waitForCommunications(0)" );
        this->radio->waitForCommunications();
    }
    
    
private:
    
    /**
     * An unique integer representing this user identification number.
     */
    const int userBoardId;
    
    /**
     * Handle all radio communications needed by this project.
     */
    Radio* radio;
    
    /**
     * Handle all USB serial communications needed by this project.
     */
    Usb* usb;
    
    /**
     * Contains the user's board configurations registered.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< LampConfiguration* > configurations;
    
    // private magic stuff
    
    
    
};







