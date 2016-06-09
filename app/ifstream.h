#ifndef IFSTREAM_H
#define IFSTREAM_H
namespace pj
{
struct ifstream
{
    ~ifstream() {}
    virtual ifstream& operator>>(int& a) {return *this;}
    virtual ifstream& operator>>(unsigned int& a) {return *this;}

    virtual ifstream& operator>>(char& a) {return *this;}

    virtual ifstream& operator>>(char* a) {return *this;}

    virtual ifstream& operator>>(double* a) {return *this;}

    virtual ifstream& operator>>(float* a) {return *this;}
    virtual void getLine(char*){}
protected:
    ifstream() {}

    void _getNext(char* next,const char* line, int& pointer)
    {
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
    }


    int atoi(const char* a){
        int mult=1;
        int i;
        int sum=0;
        int offset=0;
        while(a[offset]==' '||a[offset]=='\n')offset++;

        for(i =0; a[offset+i]>= '0' && a[ offset+ i]<='9';i++);
        i--;
        for( ; i>=0; i--){

            sum += (  a[offset+i]-'0')*mult;
            mult*=10;
        }
        return sum;

    }



};

}
#endif // IFSTREAM_H
