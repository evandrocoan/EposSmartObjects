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
#include <classes/Usb.cc>



/**
 * This class groups together the user id and the its priority used to help decide which user wins
 * the lamp configuration control.
 */
class UserRegistry
{
public:
    
    /**
     * Creates a new UserRegistry object and sets this board ID and priority.
     * 
     * @param boardId                 @see UserRegistry::userBoardId member class variable declaration/doc.
     * @param priority                @see UserRegistry::userBoardPriority member class variable declaration/doc.
     */
    UserRegistry( const int identification, const int priority ) :
                  userBoardId( identification ),
                  userBoardPriority( priority )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserRegistry::UserRegistry(2) CONSTRUCTOR! | "
                << "identification: " << identification << " priority: " << priority );
    }
    
    /**
     * Gets the current user identification number.
     * 
     * @return the current user identification number as an integer number.
     */
    const int getUserID()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserRegistry::getUserID(0)" );
        return this->userBoardId; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * Gets the current user priority.
     * 
     * @return the current user priority as an integer number.
     */
    const int getUserPriority()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE UserRegistry::getUserPriority(0)" );
        return this->userBoardPriority; // Just like a Guto's EPOS implementation.
    }
    
    
private:
    
    /**
     * An unique integer representing the user board identification number as an integer number.
     */
    const int userBoardId;
    
    /**
     * The current user board priority as an integer number.
     */
    const int userBoardPriority;
    
    // private magic stuff
    
    
    
};








