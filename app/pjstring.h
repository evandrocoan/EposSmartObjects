#ifndef STRING_H
#define STRING_H

#include "ifstream.h"
#include "ofstream.h"
namespace pj{

/*

class string
{
    public:
        string();
        string(const char*);
        string(double);
        string(int);

        pj::string operator+(const char&) const;
         pj::string operator+(const int&) const;
         pj::string operator+(const double&) const;
         pj::string operator+(const  pj::string&) const;
         pj::string operator+(const char*) const;

        void operator+=(const char) ;
        void operator+=(const int) ;
        void operator+=(const double) ;
        void operator+=(const  pj::string) ;
        void operator+=(const char*) ;

        char& operator[](const int );

        int length() const;
        char* c_str()const;

//friend pj::ofstream& operator<< ( pj::ofstream&, const  pj::string&) const;
     //   friend pj::ifstream& operator>> ( pj::ifstream&,  pj::string&);


        virtual ~string();
    protected:
        char* _str;
        int _length;
        int _truesize;

        void _append(char* c);

        ///increases _str capacity and its true size by allocing a new vector of a higher size and copying the content
        void _increaseCapacity(int n=10);

        ///sets the true size to a determined size, used to decrease unecessary alloced space
        void _setCapacity(int n );
    private:
};
*/

}

#endif // STRING_H
