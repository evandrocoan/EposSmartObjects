#ifndef RADIOCONTROL_H
#define RADIOCONTROL_H

#include "ifstream.h"
#include "ofstream.h"
#include "Message.h"
#include "Observer.h"
namespace pj
{
	class RadioControl : public Observer<Message>
	{
		public:
			RadioControl ( ifstream& receiver, ofstream& sender, int id, int size = 5 );
			void startListening();
			void stopListening();

			virtual ~RadioControl();


			void sendMessage ( const Message& ) const;

			template <class T>
			void sendMessage ( const T& t, int destiny, int type ) const;

			template <class T>
			void sendBroadCast ( const T& t, int type, int length = -1 ) const;
			int getId() const ;

		protected:
			volatile bool isListening;
			int id;
			int size;



			int lastMsg;
			ifstream& receiver;
			ofstream& sender;

			int getHash ( Message );

		private:
	};

	template <class T>
	void RadioControl::sendMessage ( const T& t, int destiny, int type ) const{
		Message m = Message ( destiny, id, type, t );
		sendMessage ( m );
	}
	template <class T>
	void RadioControl:: sendBroadCast ( const T& t, int type, int length  ) const{
		Message m = Message ( -1, id, type, t, length );
		sendMessage ( m );
	}
}
#endif // RADIOCONTROL_H
