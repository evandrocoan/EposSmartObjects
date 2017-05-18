#ifndef RADIOCONTROL_H
#define RADIOCONTROL_H

#include "ifstream.h"
#include "ofstream.h"
#include "Message.h"
#include "Observer.h"
namespace pj
{

    const short BROAD_CAST =-1;

	class RadioControl : public pj::Observer<pj::Message>
	{
		public:
			RadioControl ( ifstream& receiver, ofstream& sender, int id, int size = 5 );
			void startListening();
			void stopListening();
			virtual ~RadioControl();

			void sendMessage ( const Message& ) const;

			template <class T>
			void sendMessage ( const T& t, int destiny, int type ) ;

			template <class T>
			void sendBroadCast ( const T& t, int type, int length = -1 ) ;
			int getId() const ;

		protected:
			volatile bool isListening;
			int id;

			int lastMsg;

			short controlCounter;
			ifstream& receiver;
			ofstream& sender;

		private:
	};

	template <class T>
	void RadioControl::sendMessage ( const T& t, int destiny, int type ) {
		Message m = Message ( destiny, id, type, t );
      m.control=controlCounter;
		controlCounter++;

		sendMessage ( m );
	}
	template <class T>
	void RadioControl:: sendBroadCast ( const T& t, int type, int length  ) {
		Message m = Message ( BROAD_CAST, id, type, t, length );

		m.control=controlCounter;
		controlCounter++;

		sendMessage ( m );
	}
}
#endif // RADIOCONTROL_H
