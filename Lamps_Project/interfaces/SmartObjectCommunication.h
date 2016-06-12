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
#include <classes/Message.cc>



/**
 * This defines an interface for an smart object. Every smart object must to be able to communicates
 * with the other. To accomplish that they also need to use the same communication interface
 * compatible technology as the TCP/IP or the ARM MC13224V built-in 2.4 GHz 802.15.4 RF Radio.
 */
class SmartObjectCommunication
{
public:
    
    /**
     * 
     * 
     * @param 
     */
    virtual void sendMessage( Message ) = 0;
    
    
};




