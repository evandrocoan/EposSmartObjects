#include "serial.h"
#include "string.h"
__USING_SYS

namespace pj{
 Serial::Serial(int n, char control):control(control){
	this->uart = new UART();
	MSG_LEN =n;

}


     ofstream& Serial::operator<<(const int& a){
        cout<<a;

         return *this;

     }
	ofstream& Serial::operator<<(const char* a) {
	      cout<<a;
         return *this;
	}

     ofstream& Serial::operator<<(const char a){
           cout<<a;
           return *this;

     }
     ofstream& Serial::operator<<(const unsigned int a) {
           cout<<a;
           return *this;

     }
     ofstream& Serial::operator<<(const float a) {

           return *this;
     }

     ofstream& Serial::operator<<(const double a) {

           return *this;
     }



void Serial::readInput() {

	unsigned int i;
	char msg[MSG_LEN];


		do {
			msg[0] = uart->get();
		} while (msg[0] != control);
		i = 0;
		do{
			msg[i] = uart->get();
			i++;
		}
		while ((msg[i - 1] != '\n') && (msg[i-1] != 13) && (i < MSG_LEN)) ;

		memset(msg + i, 0x00, MSG_LEN - i);
		// message received.
		strcpy(inputBuffer,msg);
		pointer=0;


}


}
