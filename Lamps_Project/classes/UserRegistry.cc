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
 * This class groups together the user id and the its priority used to help decides which user wins
 * the lamp configuration control.
 */
class UserRegistry
{
public:
    
    /**
     * To creates a new lampBoard object and sets this board ID and priority.
     * 
     * @param boardId                 the current board user ID as an integer number.
     * @param configuration           the LampConfigurationStrategy default lamp configuration object.
     */
    UserRegistry( const int, const int );
    
    /**
     * Gets the current user identification number.
     * 
     * @return the current user identification number as an integer number.
     */
    const int getUserID();
    
    /**
     * Gets the current user priority.
     * 
     * @return the current user priority as an integer number.
     */
    const int getUserPriority();
    
    
private:
    
    /**
     * An unique integer representing the user board identification number as an integer number.
     */
    const int userBoardId;
    
    /**
     * The current user board priority as an integer number.
     */
    const int userBoardPriority;
    
    /**
     * Disables the default constructor.
     */
    UserRegistry();
    
};



/**
 * @see UserRegistry::UserRegistry() member class declaration.
 */
UserRegistry::UserRegistry()
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserRegistry::UserRegistry(0) THE DISABLED CONSTRUCTOR!" );
}

/**
 * @see UserRegistry::UserRegistry() member class declaration.
 */
UserRegistry::UserRegistry( const int identification, const int configuration ) :
userBoardId( identification ),
userBoardPriority( configuration )
{
    DEBUGGERLN( 2, "I AM ENTERING ON THE UserRegistry::UserRegistry(2) CONSTRUCTOR!" );
}





