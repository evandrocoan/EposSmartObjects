#include "RadioControl.h"

namespace pj
{

	RadioControl::RadioControl ( ifstream& receiver, ofstream& sender, int id, int size) :
		Observer<Message> ( size ), id ( id ),
		sender ( sender ), receiver ( receiver )
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
			receiver >> m;

			///retorna se mensagem vier dele mesmo;
			if(m.getOrig() == id)return;

            int hash = getHash ( m );

            ///returns if its the same message as last
            if(hash == lastMsg)return;


            if( m.getDest() == id){
                notifyListeners ( m );
                lastMsg == hash;
                return;
            }
            if(m.getDest() == BROAD_CAST){
                //TODO: todo finish radius broadcast
                notifyListeners ( m );
                lastMsg == hash;
                sendMessage ( m );
                return;
            }
            ///if message is not for user
            lastMsg == hash;
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
	int RadioControl::getHash ( Message m )
	{
		int first = m.getDest();
		first = ( first << 24 )  & ( 255 << 24 ) ;

		int second = m.getOrig();
		second = ( second << 16 ) & ( 255 << 16 ) ;

		int third = ( ( int ) m.getData() );
		third = third & ( ( 128 * 128 - 1 ) << 2 );

		int last = m.getTipo();
		last = last & ( 3 );

		return ( first + second + third + last );
	}

}







