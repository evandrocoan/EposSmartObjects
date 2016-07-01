#ifndef RADIO_PJ
#define RADIO_PJ

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
 * This class implements the EPOSMotes 2 radio use. It radio is an ARM MC13224V built-in 2.4 GHz
 * 802.15.4 RF Radio.
 * 
 * When the radio receives a message it should notify its observer by calling its inherited subject
 * object subject.
 * 
 * @see CommunicationSubject::notifyObserver( const char* ) member class declaration for the subject
 *      object use.
 */
 

#include <nic.h>
#include "ifstream.h"
#include "ofstream.h"
__USING_SYS

namespace pj{
 
class Radio : public ifstream, public ofstream
{
public:

	Radio(short bufferSize=50){
		BUFFERSIZE=bufferSize;
		radioNicController = new NIC();
		buffer = new char[BUFFERSIZE];
	}
    
     ofstream& operator<<(const char* a) {
	  	
     	int i=0;
     	while(a[i])i++;
     	sendBytes(a, i);
     	return *this;
     }
     
     
    void sendBytes(const void* message, int size   )
    {
    	
          radioNicController->send( 1, 0, message, size );
         
    }
 protected:
    char* buffer;
    int receiver()
    {
        NIC::Protocol prot;
        NIC::Address  src;
        
        
        
            while ( !( radioNicController->receive( &src, &prot, this->buffer,  BUFFERSIZE  ) > 0 ) );
            
            this->buffer[BUFFERSIZE-1]=0;
         
        
        return 0;
    }
    
private:
	short BUFFERSIZE;
    
     
     /**
     * Gets the only allowed Radio's instance by lazy initialization.
    */
    
    /**
     * @see SmartObjectCommunication::sendMessage( const char* ) member class declaration.
     */
    
   
    /**
     * The EPOS radio controller.
     * 
     * @see <http://epos.lisha.ufsc.br/EPOS+User+Guide#NIC>
     */
     NIC* radioNicController;
    
    
    /**
     * Creates an radio object ready to be used by the singleton design pattern.
     */
    
 
     
     
    
    
     char* getBuffer(){
     	return this->buffer;
     }


     /**void readInput()
        read from whatever source, make sure to update pointer to the buffer and your inputbuffer accordingly
    */
	 void readInput(){
	 	receiver();
	}
	 

    /**int getBufferLenght()
        return the maximum size of the buffer
    */
	 int getBufferLength(){
	 	return BUFFERSIZE;
	 }
    
    
    
    /**
     * Do not implement it. Disable the copy constructor to avoid multiple singleton object copies.
     */
    Radio( Radio const& );
    
    /**
     * Do not implement it. Disable the assignment operator to avoid multiple singleton object copies.
     */
    void operator=( Radio const& );
    
    // radio magic methods
    
    
    
};




}


#endif
