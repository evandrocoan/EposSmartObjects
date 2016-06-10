#ifndef OFSTREAM_H
#define OFSTREAM_H

namespace pj{

const char endl[] ={ '\n', 13};

struct ofstream{

    virtual    ~ofstream(){}

	virtual ofstream& operator<<(const int& a) {return *this;}
	virtual ofstream& operator<<(const char* a) {return *this;}

    virtual ofstream& operator<<(const char a){return *this;}
    virtual ofstream& operator<<(const unsigned int a)  {return *this;}
    virtual ofstream& operator<<(const float a)  {return *this;}

    virtual ofstream& operator<<(const double a)  {return *this;}



	protected:
	    ofstream() {}



};

}

#endif
