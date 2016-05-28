
#ifndef SERIAL_WAG
#define SERIAL_WAG

#include <uart.h>
#include "ofstream.h"
#include "ifstream.h"
namespace pj{

struct Serial: public pj::ofstream, public pj::ifstream{

	  Serial(int n=20, char control='?');
	  ~Serial(){}


     ifstream& operator>>(int& a);
     ifstream& operator>>(unsigned int& a);

     ifstream& operator>>(char& a);

     ifstream& operator>>(char* a);

     ifstream& operator>>(double* a);

     ifstream& operator>>(float* a);

     ofstream& operator<<(const int& a);
	 ofstream& operator<<(const char* a) ;

     ofstream& operator<<(const char a);
     ofstream& operator<<(const unsigned int a)  ;
     ofstream& operator<<(const float a)  ;

     ofstream& operator<<(const double a)  ;
     
      void getLine(char*);

	 protected:

        __SYS_NS::OStream cout;
        char* inputBuffer;
        int pointer;
        unsigned int MSG_LEN ;
        char control;

         void getNextInput(char* input);

	     int readInput();


};

}
#endif
