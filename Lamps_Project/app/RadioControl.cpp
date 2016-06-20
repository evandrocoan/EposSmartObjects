#include "RadioControl.h"

namespace pj{

RadioControl::RadioControl(ifstream& receiver,ofstream& sender, int size ):
    sender(sender),receiver(receiver)
    {
    web = new int[size];
   id= generateId();
}

RadioControl::~RadioControl()
{
    //dtor
}

 void RadioControl::startListening(){
     isListening = true;
     while(isListening){
        Message m;
        receiver>>m;

        if(m.getDest() == id ){
            notifyListeners(m);
        }else{
            int hash = getHash(m);
            if(hash != lastMsg){
                lastMsg==hash;
                sendMessage(m);
            }

        }
     }

 }
void RadioControl::stopListening(){
    while(isListening==true)isListening=false;
}
 int RadioControl::sendMessage(Message m){
        sender<<m;
 }
int RadioControl::getId(){
    return this->id;
}
///hash = 1byte destiny, 1byte origin, 14bits data, 2 bits type
 int RadioControl::getHash(Message m){

        int first = m.getDest();
        first = (first <<24)  & (255<<24) ;

        int second = m.getOrig();
        second = (second <<16) & (255<<16) ;

        int third =((int) m.getRest());
        third = third & ((128*128-1)<<2);

        int last = m.getTipo();
        last = last & (3);

        return (first + second + third + last);
 }
 int RadioControl::generateId(){
    //TODO :rand
    return 0;
 }




}







