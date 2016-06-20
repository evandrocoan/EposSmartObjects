

#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <errno.h>

#include "Interface.h"



/**
 * This is an abstract class which aggregates to the SmartObject interface the 
 */
class Abstract : public Interface
{
public:
    
    static const char* message;
    
    /**
     * 
     * 
     * @param 
     */
    virtual void sendMessage( const char* ) = 0;
    
};

const char* Abstract::message = "Hi";


