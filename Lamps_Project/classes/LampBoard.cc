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
#include <classes/UserRegistry.cc>
#include <classes/PwmHardware.cc>
#include <classes/LampConfiguration.cc>



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
    LampBoard( const int boardId, LampConfiguration* configuration ) :
           lampBoardId( 0 ),
           defaultConfiguration( NULL ),
           controlStrategies(),
           priorityUsers()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::LampBoard(2) CONSTRUCTOR!" );
    }
    
    /**
     * @see CommunicationStrategyObserver::receiveMessage abstract member class declaration.
     */
    void receiveMessage( const char* message )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::receiveMessage(1) | message: \n" << message );
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
        
        return false;
    }
    
    /**
     * Gets the current board unique board identification number.
     * 
     * @return this lamp board identification as a const integer.
     */
    const int getLampBoardId()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::getLampBoardId(0)" );
        
        return 0; // Just like a Guto's EPOS implementation.
    }
    
    
private:
    
    /**
     * An unique integer representing this lamp identification number.
     */
    const int lampBoardId;
    
    /**
     * The default lamp configuration used the there is no user around.
     */
    LampConfiguration* defaultConfiguration;
    
    /**
     * Contains the control strategies this board is using.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< LampControlStrategy > controlStrategies;
    
    /**
     * Contains the user's which have priority over non-registered users.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< UserRegistry > priorityUsers;
    
    /**
     * Disables the default constructor.
     */
    LampBoard() :
           lampBoardId( 0 ),
           defaultConfiguration( NULL ),
           controlStrategies(),
           priorityUsers()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampBoard::LampBoard(0) THE DISABLED CONSTRUCTOR!" );
    }
    
    // private magic stuff
    
    
    
};







