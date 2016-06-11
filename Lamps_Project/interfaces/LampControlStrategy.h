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
#include <interfaces/Lamp.h>



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
 * This class is the interface to an strategy to control the Lamp's objects.
 */
class LampControlStrategy
{
public:
    
    /**
     * Adds a new lamp into the controlling system.
     * 
     * @param lamp              the lamp to be updated/receive its new setting.
     * @param newSetting        an LampConfigurationStrategy configuration object.
     */
    virtual void addNewLamp( Lamp* lamp, LampConfigurationStrategy* setting ) = 0;
    
    /**
     * Given one lamp, changes its configuration.
     * 
     * @param lamp              the lamp to be updated/receive its new setting.
     * @param newSetting        an LampConfigurationStrategy configuration object.
     */
    virtual void setNewUserSettings( Lamp* lamp, LampConfigurationStrategy* newSetting ) = 0;
    
    
protected:
    
    /**
     * Contains the lamps this strategy is controlling.
     * 
     * @see LISHA's website <http://epos.lisha.ufsc.br/EPOS+User+Guide#Simple_Ordered_List>
     */
    Ordered_List< Lamp* > lamps;
    
};




