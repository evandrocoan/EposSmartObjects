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
#include <classes/UserRegistry.cc>



/**
 * #define __SYS_NS	   System
 * #define __USING_SYS using namespace __SYS_NS
 */
__USING_SYS;



/**
 * This class implementes the Lamp's objects using the LampConfiguration abstract class.
 */
struct LampConfiguration
{
public:
    
    /**
     * Whether the lamp is to fade out when the user get out the lamp environment/area.
     */
    bool isToFadeOut;
    
    /**
     * Whether the lamp is to fade in when the user get in the lamp environment/area.
     */
    bool isToFadeIn;
    
    /**
     * Specifies special behaviors allowed as party style.
     */
    const char* const specialFlags;
    
    /**
     * Creates an configuration initializing the constant and private class attributes.
     * 
     * @param lampType          see LampConfiguration::lampType public constant class atribute.
     * @param specialFlags      see LampConfiguration::specialFlags public constant class atribute.
     * @param maximumBright     see LampConfiguration::maximumBright private class atribute.
     * 
     * @see LampConfiguration::isToFadeOut public class atribute.
     * @see LampConfiguration::isToFadeIn public class atribute.
     */
    LampConfiguration( const char* const specialFlags, int maximumBright ) :
            specialFlags( specialFlags )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::LampConfiguration(0) CONSTRUCTOR!" );
        
        this->isToFadeOut = isToFadeOut;
        this->isToFadeIn  = isToFadeIn;
        
        if( !this->setBright( maximumBright ) )
        {
            DEBUGGERLN( 1, "ERROR! Could not set the bright correctly! maximumBright: " << maximumBright );
        }
    }
    
    /**
     * Gets the maximum bright desired to the lamp achieve.
     * 
     * @return an integer as the desired bright to the lamp. This is changed by setBright(int).
     * @see LampConfiguration::maximumBright private class atribute.
     */
    int getBright()
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::getBright(0)" );
        
        return this->maximumBright; // Just like a Guto's EPOS implementation.
    }
    
    /**
     * Adjust the maximum bright desired to the lamp achieve. Usually a value between 0 and 100.
     * 
     * @param newBright              an integer as the new desired bright to the lamp.
     * @see LampConfiguration::maximumBright private class atribute.
     */
    bool setBright( int brigth )
    {
        DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::setBright(1) | brigth: " << brigth );
        
        if( brigth > -1
            && brigth < 101 )
        {
            this->maximumBright = brigth;
            return true;
        }
        
        return false; // Just like a Guto's EPOS implementation.
    }
    
    
private:
    
    /**
     * The maximum bight allowed to this lamp. It is a value between 0 and 100, as in percents.
     */
    int maximumBright;
    
    
};











