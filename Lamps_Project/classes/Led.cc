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
 * DO NOT MODIFIY IT!
 * 
 * @see The Class LampBoard main include declaration/explanation.
 */
#include <interfaces/Lamp.h>



/**
 * This class implements Led lamps using the abstract class Lamp.
 */
class Led : public Lamp
{
public:
    
    /**
     * 
     */
    Led( LampConfiguration* configuration )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::Led(0) THE CONSTRUCTOR!" );
        
        this->configuration = configuration;
    }
    
    /**
     * @see Lamp::setNewUserSetting( LampConfiguration ) member abstract class declaration.
     */
    bool setNewUserSetting( LampConfiguration* newSetting )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::setNewUserSetting(1)" );
        
        if( this->configuration == NULL )
        {
            this->configuration = newSetting;
            
            DEBUGGERLN( 1, "    ( setNewUserSetting ) Returning true." );
            return true;
        }
        else if( newSetting != NULL )
        {
            if( !compareCharArray( newSetting->lampType, this->configuration->lampType ) )
            {
                delete configuration;
                this->configuration = newSetting;
                
                DEBUGGERLN( 1, "    ( setNewUserSetting ) Returning true." );
                return true;
            }
        }
        
        DEBUGGERLN( 1, "    ( setNewUserSetting ) Returning false." );
        return false;
    }
    
    /**
     * @see Lamp::getCurrentBright() member abstract class declaration.
     */
    int getCurrentBright()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE Led::getCurrentBright()" );
        return this->configuration->getBright();
    }
    
    /**
     * @see Lamp::getLampType() member abstract class declaration.
     */
    virtual const char* getLampType()
    {
        return this->configuration->lampType;
    }
    
    
private:
    
    // private magic stuff
    
    
    
};









