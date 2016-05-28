
bool led1=true;

__USING_SYS

OStream cout;



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

int ReceiveCommandUART(char* tosend) {
	cout << "Thread UART initing\n";
	unsigned int i;
	char msg[20]; //[DATA_SIZE];

	UART * uart = new UART();

	
		do {
			msg[0] = uart->get();
		} while (msg[0] != '?'); 
		i = 0;

		while ((msg[i - 1] != '\n') && (msg[i-1] != 13) && (i < 20)) {
			msg[i++] = uart->get();
		}
		memset(msg + i, 0x00, 20 - i);
		// message received.
		strcpy(tosend,msg);
		
	cout << "Thread UART finishing\n";
	return 0;
}



int main(){
 
	
	char he3[]="hhhhhhhhhhhhhhhhhh";

	while(true){
	 	wait(2000);
		cout<<"readyup\n";
	    toggle_on_led1() ;
	    
	   ReceiveCommandUART(he3);
	   cout<<he3;
	   cout<<"\n";
 	}
	return 0;


}
