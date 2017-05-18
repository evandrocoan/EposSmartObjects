#include "RadioControl.h"

#include "global.h"
namespace pj
{

	RadioControl::RadioControl ( ifstream& receiver, ofstream& sender, int id, int size ) :
		Observer<Message> ( size ), id ( id ),
		sender ( sender ), receiver ( receiver ), controlCounter(0)
	{


	}

	RadioControl::~RadioControl()
	{
		//dtor
	}

	void RadioControl::startListening()
	{


	 	isListening = true;
		while ( isListening ) {
			Message m;
			m.origin = -1337;
			receiver >> m;

           ///retorna se mensagem vier dele mesmo;
			if(m.getOrig() == id)continue;

            int hash = m.getHash ( );


            ///returns if its the same message as last
            if(hash == lastMsg)continue;




            if( m.getDest() == id){
                notifyListeners ( m );
                lastMsg = hash;
                continue;
            }
            if(m.getDest() == BROAD_CAST){
                if(m.reach <= -1 ){
                    notifyListeners ( m );
                    lastMsg = hash;
                    sendMessage ( m );
                    continue;
                }
                if(m.reach > 0){
                    m.reach--;
                    notifyListeners ( m );
                    lastMsg = hash;
                    sendMessage ( m );
                    continue;
                }
                ///else reach == 0,stop the relay.
                continue;

            }
            ///if message is not for user
            lastMsg = hash;
            sendMessage ( m );

		}



	}
	void RadioControl::stopListening()
	{
		while ( isListening == true ) { isListening = false; }
	}
	void RadioControl::sendMessage ( const Message& m ) const
	{

		sender << m;
	}
	int RadioControl::getId() const
	{
		return this->id;
	}
///hash = 1byte destiny, 1byte origin, 14bits data, 2 bits type


}







