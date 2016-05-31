
#include "ifstream.h"
#include "ofstream.h"

#ifndef RADIO
#define RADIO

namespace pj{
struct Radio:public pj::ofstream, public pj::ifstream{
	Radio(int length=20);
	~Radio();
	  

     ofstream& operator<<(const int& a);
	 ofstream& operator<<(const char* a) ;

     ofstream& operator<<(const char a);
     ofstream& operator<<(const unsigned int a)  ;
     ofstream& operator<<(const float a)  ;

     ofstream& operator<<(const double a)  ;

      void getLine(char*);

  protected:
    unsigned int length;
    char* inputBuffer;
    
     char* getBuffer(){return inputBuffer;}
	 void readInput();
	 int getBufferLength(){return length;}
   

}


}
