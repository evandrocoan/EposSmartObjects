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
 * This class is the interface used to allow the observers to be called when the observed object
 * is changed. The classes which want to be notified when the observed object changes, must to
 * extend this abstract class and implements its methods.
 */
class CommunicationStrategyObserver
{
public:
    
    /**
     * This defines the communication observer notifier function to be used by this project concrete
     * classes. This function is responsible for interpret the received messages by the board, and
     * take the properly action.
     * 
     * @see SmartObjectCommunication interface declaration.
     */
    virtual void receiveMessage( Message ) = 0;
    
    
};





