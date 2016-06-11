#include <alarm.h>
#include <machine.h>
#include <nic.h>
#include <utility/ostream.h>
#include <sensor.h>

__USING_SYS

const unsigned char SINK_ID = 0x01;

struct LampConfiguration
{
public:
    
    /**
     * Whether the lamp is to fade when the user get out the lamp environment/area.
     */
    bool isToFadeOut;
    
    /**
     * Whether the lamp is to fade when the user get out the lamp environment/area.
     */
    bool isToFadeIn;
    
    /**
     * This is to what lamp this configuration to apply. Example: blueLed, redLed, partyColors.
     */
    const char* const lampType;
    
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
    LampConfiguration( const char* const lampType, const char* const specialFlags, int maximumBright ) :
            lampType( lampType ),
            specialFlags( specialFlags )
    {
       // DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::LampConfiguration(0) THE CONSTRUCTOR!" );
        
        this->isToFadeOut = isToFadeOut;
        this->isToFadeIn  = isToFadeIn;
        
        if( !this->setBright( maximumBright ) )
        {
           // DEBUGGERLN( 1, "ERROR! Could not set the bright correctly! maximumBright: " << maximumBright );
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
        //DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::getBright(0)" );
        
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
        //DEBUGGERLN( 2, "I AM ENTERING ON THE LampConfiguration::setBright(1) | brigth: " << brigth );
        
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

void sender(char id[10]) {
    NIC nic;

    unsigned char src, prot;
    unsigned int size;

    LampConfiguration config ("eu", "presto", 100);

    int i;
    while (true) {
        for (i = 5; i < 8; i++) {
            //CPU::out8(Machine::IO::PORTB, (1 << i));

	    config.setBright(69);

            nic.send(SINK_ID, 0, &config, sizeof(config));

            Alarm::delay(100000);
        }
    }
}

int receiver() {
	NIC nic;
    NIC::Protocol prot;
    NIC::Address src;

    LampConfiguration config ("eu", "presto", 100);

    OStream cout;

    //unsigned char src, prot;
    //int i;
    
    cout << "Sink\n";

    while (true) {
        while (!(nic.receive(&src, &prot, &config, sizeof(config)) > 0));

        cout << "####################\n";
	cout <<"Configured brightness: " << config.getBright() << "\n";
	cout << "Lamp type: " << config.lampType << "\n";
	cout << "Flags: " << config.specialFlags << "\n";
    }
	return 0;
}

int main() {
	char mensagem[10] = {'H', 'e', 'l', 'l', 'o', 'W', 'o', 'r', 'l', 'd'};
    sender(mensagem);
   // receiver();
}
