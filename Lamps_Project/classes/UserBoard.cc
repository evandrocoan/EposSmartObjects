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

#include <utility/malloc.h>
#include <utility/list.h>
#include <headers/lamps_project_debugger.h>
#include <headers/array_operations.h>
#include <interfaces/CommunicationStrategyObserver.h>



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
 * This class is the interface used to allow the observers to be called when the observed object
 * is changed. The classes which want to be notified when the observed object changes, must to
 * extend this abstract class and implements its methods.
 */
class UserBoard : private CommunicationStrategyObserver
{
public:
    
    /**
     * To creates a new userBoard object and sets this board ID and priority.
     * 
     * @param boardId             the current board user ID as an integer number.
     * @param configuration       the first and initial user's board configuration to use.
     */
    UserBoard( const int, LampConfigurationStrategy );
    
    /**
     * @see CommunicationStrategyObserver::receiveMessage abstract member class declaration.
     */
    void receiveMessage( const char* );
    
    /**
     * Gets the current board unique user identification number.
     * 
     * @return this user board identification as a const integer.
     */
    const int getUserId();
    
    /**
     * Add a new user's configuration to this user's board.
     * 
     * @param configuration          the user's new configuration to add.
     * @return true when the user configuration is successfully added, otherwise false.
     */
    bool addUserConfiguration( LampConfigurationStrategy );
    
    /**
     * Gets a specified user's configuration by a given index.
     * 
     * @param index           the user's configuration index as an integer value.
     * @return the specified user's configuration. NULL when an invalid index is provided.
     */
    LampConfigurationStrategy getUserConfiguration( int );
    
    /**
     * Set a specified user's configuration as the current active configuration ready to be passed
     * to the LampBoard as soon as the user approach one lamp device.
     * 
     * @param index           the user's configuration index as an integer value.
     * @return true when the specified user's configuration is set as the current configuration,
     *         otherwise false.
     */
    bool setUserConfiguration( int );
    
    
private:
    
    /**
     * An unique integer representing this user identification number.
     */
    const int userBoardId;
    
    /**
     * Contains the user's board configurations registered.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< LampConfigurationStrategy > configurations;
    
    /**
     * Disables the default constructor.
     */
    UserBoard();
    
};



/**
 * @see UserBoard::UserBoard() member class declaration.
 */
UserBoard::UserBoard()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::UserBoard(0) THE DISABLED CONSTRUCTOR!" );
}

/**
 * @see UserBoard::UserBoard( const int ) member class declaration.
 */
UserBoard::UserBoard( const int userId) : userBoardId( userId )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::UserBoard(1) CONSTRUCTOR!" );
}

/**
 * @see UserBoard::receiveMessage() member class declaration.
 */
void UserBoard::receiveMessage( const char* message )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::receiveMessage(1) | message: \n" << message );
}

/**
 * @see UserBoard::getUserId() member class declaration.
 */
const int UserBoard::getUserId()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserBoard::getUserId() | userBoardId: " << this->userBoardId );
    return this->userBoardId;
}



