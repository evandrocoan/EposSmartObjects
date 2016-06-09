#include <string.h>

#ifndef IFSTREAM_H
#define IFSTREAM_H
namespace pj
{
struct ifstream
{
    ~ifstream() {}
   virtual ifstream& operator>>(int& a){
         char buffer[10];
         getNextInput(buffer);
         a = atoi(buffer);
        return *this;
     }
    virtual ifstream& operator>>(unsigned int& a){
           char buffer[10];
         getNextInput(buffer);
         a = atoi(buffer);
        return *this;

     }

    virtual ifstream& operator>>(char& a){
        char* inputBuffer = getBuffer();
        while(inputBuffer[pointer] != 0 && (inputBuffer[pointer]== '\n' || inputBuffer[pointer]==' '))
        {
            pointer++;
        }
        if(pointer>= getBufferLength() -1 || inputBuffer[pointer]==0){
            pointer=0;
            readInput();
        }

        a =inputBuffer[pointer];
        pointer++;
        return *this;

     }

     virtual ifstream& operator>>(char* a){
        char buffer[getBufferLength()];
        getNextInput(buffer);
        strcpy(a , buffer);
       return *this;
     }

     virtual ifstream& operator>>(double& a){

     	   char buffer[30];
         getNextInput(buffer);
         a = atof(buffer);
        return *this;
     }

    virtual ifstream& operator>>(float& a){

     	 char buffer[30];
         getNextInput(buffer);
         a = atof(buffer);
        return *this;
     }
     virtual char* getLine(char* a){
         char* inputBuffer = getBuffer();
         while(inputBuffer[pointer] != 0 && (inputBuffer[pointer]== '\n' || inputBuffer[pointer]==' '))
        {
            pointer++;
        }
        if(pointer>= getBufferLength() -1){
            pointer =0;
            readInput();
        }

         int i=0;
         while (inputBuffer[pointer] != '\0' && inputBuffer[pointer] !='\n'){
            a[i] = inputBuffer[pointer];
            i++;
            pointer++;
         }
         a[i]= 0;
         return a;
    }

protected:
    ifstream() {pointer =0;}

    ///these methods must be implemented if using a inputbuffer system

    /**char* getBuffer()
        return a pointer to the inputbuffer
    */
    virtual char* getBuffer()=0;


     /**void readInput()
        read from whatever source, make sure to update pointer to the buffer and your inputbuffer accordingly
    */
	virtual void readInput()=0;

    /**int getBufferLenght()
        return the maximum size of the buffer
    */
	virtual int getBufferLength()=0;
	int pointer;

    void _getNext(char* next)
    {
        char *line = getBuffer();
        int i=0;

        while(line[pointer] != 0 && (line[pointer]== '\n' || line[pointer]==' '))
        {
            pointer++;
        }
        while(line[pointer] != 0 && line[pointer]!= '\n' && line[pointer]!=' ')
        {
            next[i]= line[pointer];
            i++;
            pointer++;
        }
        pointer++;
        next[i]=0;
        pointer--;
    }

   virtual void getNextInput(char* input){
        _getNext(input);
        int siz= strlen(input);

        if(siz==0){
            pointer=0;
            readInput();
            getNextInput(input);
        }

    }


  double atof(char *str){
    double res = 0;
    int sign = 1;
    int i = 0;
    if (str[0] == '-')
    {
        sign = -1;
        i++;  // Also update index of first digit
    }
    for (; str[i] >='0' && str[i]<='9'; ++i){
        res = res*10 + str[i] - '0';
    }
    if(str[i]=='.'){
        i++;
        double c=10;

          for (; str[i] >='0' && str[i]<='9'; ++i){
            res += (str[i] - '0')/c;
            c*=10;
          }
    }
    return sign*res;
	}
	int atoi(char *str)
	{
		int res = 0;
		int sign = 1;
		int i = 0;

		if (str[0] == '-')
		{
		    sign = -1;
		    i++;  // Also update index of first digit
		}
		for (; str[i] >='0' && str[i]<='9'; ++i)
		    res = res*10 + str[i] - '0';
		return sign*res;
	}



};

}
#endif // IFSTREAM_H
