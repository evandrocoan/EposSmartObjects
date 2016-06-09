
#ifndef SERIAL_WAG
#define SERIAL_WAG

#include <uart.h>
#include "ofstream.h"
#include "ifstream.h"
namespace pj{

struct Serial: public pj::ofstream, public pj::ifstream{

	  Serial(int n=20, char control=':');
	  ~Serial(){}


     

     ofstream& operator<<(const int& a);
	 ofstream& operator<<(const char* a) ;

     ofstream& operator<<(const char a);
     ofstream& operator<<(const unsigned int a)  ;
     ofstream& operator<<(const float a)  ;

     ofstream& operator<<(const double a)  ;
     
    

	 protected:
	 
        __SYS_NS::OStream cout;
        __SYS_NS::UART * uart;
        char* inputBuffer;
      
        unsigned int MSG_LEN ;
        char control;
        
       char* getBuffer(){return inputBuffer;}
	   void readInput();
	   int getBufferLength(){return MSG_LEN;}

      

	    


};

}
#endif
