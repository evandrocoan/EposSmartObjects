//#include <uart.h>

//#include "serial.h"

#include <uart.h>
#include <nic.h>
__USING_SYS


bool led1=true;
OStream cout;
NIC *nic;


unsigned int *GPIO_BASE = (unsigned int*)0x80000000;


 static void wait(unsigned int us) {
	static volatile long i=0;
	long clock=	1500;
	static volatile int plus=1;
	for(i=0 ; i< us*clock; )i+=plus;
   }

void toggle_on_led1() {
	if(!led1){
		*GPIO_BASE = *GPIO_BASE | (1 << 23);
		led1=true;
	}else{
		*GPIO_BASE = *GPIO_BASE & !(1 << 23);
		led1=false;
	}
}

char msg[20]={0};

char msg4[]="hellold3\n";
char msg3[]="hellold2\n\xd";

int ReceiveCommandNIC() {
	// semcout->p();
	//cout << "Thread NIC initing\n";
	// semcout->v();

	msg[20];

	NIC::Protocol prot;
	NIC::Address src;

	while (!(nic->receive(&src, &prot, &msg, sizeof(msg)) > 0)) {
			// semcout->p();
			//cout << ".";
			// semcout->v();
		}
		// semcout->p();
	//	cout << "\nMessage received: " << msg << "\n";
		// semcout->v();
		
	

	cout << "Thread NIC finishing\n";
	return 0;
}


void SendMessageToNIC(char msg2[20]) {
	int r;

	while ((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg2, sizeof(msg2)))
			== 0) // != 11 ?
	{ // semcout->p();
		cout << "Send failed " << r << "\n";
		// semcout->v();
	}

	// semcout->p();
	cout << "Message sent\n";

	// semcout->v();
}
void sender(){
	static int i=0;
	i++;
	i= i%3;
	char msg5[]="01234567891234567";
	SendMessageToNIC("fuck you\n");
	switch(i){
		case 0:{
			SendMessageToNIC(msg3);
			break;
		}
		case 1:{
			SendMessageToNIC(msg4);
			break;
		}
		case 2:{
			SendMessageToNIC(msg5);
			break;
		}
	}
	
}
void receiver(){
	
	ReceiveCommandNIC();
	
	cout<<" message is:" <<msg<<":end\n";
}


int main(){
 	nic = new NIC();
	while(true){
	 	cout<<"ready\n";
	    wait(2000);
	  sender();
	   // receiver();
	   
 	}
	return 0;


}
