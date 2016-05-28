#include "serial.h"
#include "string.h"
__USING_SYS

namespace pj{
 Serial::Serial(int n, char control):control(control){
	//this->uart = new UART();
	inputBuffer = new char(n);
	MSG_LEN = n;
}

     ifstream& Serial::operator>>(int& a){
         char buffer[10];
         getNextInput(buffer);
         a = atoi(buffer);
        return *this;
     }
     ifstream& Serial::operator>>(unsigned int& a){
           char buffer[10];
         getNextInput(buffer);
         a = atoi(buffer);
        return *this;

     }

     ifstream& Serial::operator>>(char& a){


        while(inputBuffer[pointer] != 0 && (inputBuffer[pointer]== '\n' || inputBuffer[pointer]==' '))
        {
            pointer++;
        }
        if(pointer>= MSG_LEN -1){
            readInput();
        }

        a =inputBuffer[pointer];
        pointer++;
        return *this;

     }

     ifstream& Serial::operator>>(char* a){
        char buffer[MSG_LEN];
        getNextInput(buffer);
        strcpy(a , buffer);
       return *this;
     }

     ifstream& Serial::operator>>(double* a){
        return *this;
     }

     ifstream& Serial::operator>>(float* a){
        return *this;
     }
     void Serial::getLine(char* a){
         while(inputBuffer[pointer] != 0 && (inputBuffer[pointer]== '\n' || inputBuffer[pointer]==' '))
        {
            pointer++;
        }
        if(pointer>= MSG_LEN -1){
            readInput();
        }

         int i=0;
         while (inputBuffer[pointer] != '\0' && inputBuffer[pointer] !='\n'){
            a[i] = inputBuffer[pointer];
            i++;
            pointer++;
         }
         a[i]= 0;


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



int Serial::readInput() {

	unsigned int i;
	char msg[MSG_LEN]; //[DATA_SIZE];

	UART * uart = new UART();
		do {
			msg[0] = uart->get();
		} while (msg[0] != control);
		i = 0;

		while ((msg[i - 1] != '\n') && (msg[i-1] != 13) && (i < MSG_LEN)) {
			msg[i++] = uart->get();
		}
		memset(msg + i, 0x00, MSG_LEN - i);
		// message received.
		strcpy(inputBuffer,msg);
		pointer=0;
		delete(uart);
	return i;
}

void Serial::getNextInput(char* input){
     _getNext(input, inputBuffer, pointer);
    int siz= strlen(input);

    if(siz==0){
        readInput();
        getNextInput(input);
    }
}

}
