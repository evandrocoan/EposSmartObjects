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
#include <classes/Radio.cc>



/**
 * This class is the abstract class to the Lamp's objects.
 */
class Lamp
{
public:
    
    Lamp() : lampType( "BlueLed" )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Lamp::Lamp(0) THE CONSTRUCTOR! lampType: " << this->lampType );
    }
    
    /**
     * Adds a new lamp into the controlling system.
     * 
     * @param setting        an LampConfiguration configuration object.
     */
    virtual bool setNewUserSetting( LampConfiguration* newSetting ) = 0;
    
    /**
     * Reads the lamp configuration and get it current desired bright.
     * 
     * @return an int as the current lamp bright.
     */
    virtual int getCurrentBright() = 0;
    
    /**c
     * Get the current lamp type. Example: BlueLed.
     * 
     * @return an const char* as the current lamp type.
     */
    virtual const char* getLampType() = 0;
    
    /**
     * Reads the lamp configuration and get it current lamp special flags. Example: "use_light_sensor".
     * 
     * @return an const char* as the current lamp special flags.
     */
    virtual const char* getSpecialFlags() = 0;
    
    
protected:
    
    /**
     * This is to what lamp this configuration to apply. Example: blueLed, redLed, partyColors.
     */
    const char* const lampType;
    
    /**
     * Contains this lamp configuration.
     */
    LampConfiguration* configuration;
    
};







