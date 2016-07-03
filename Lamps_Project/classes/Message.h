#ifndef MENSAGEM
#define MENSAGEM

#include "ofstream.h"
#include "ifstream.h"

#define MESSAGESIZE 38


namespace pj
{
   class RadioControl;


	class Message{

		private:
		    friend RadioControl;

			short destiny;
			short origin;
			short type;
			char size;
            short reach;
            short control;
			char data[MESSAGESIZE];



		public:
			Message()
			{
                if(origin>2){
                    int a=2;
                }

			}



			Message ( short destino, short origem, short type, void* data, short size, short reach = -1 )
				:  destiny ( destino ), origin ( origem ), type ( type ), size ( size )
			{
			     if(origin>2){
                    int a=2;
                }
				for ( int i = 0; i < size && i < MESSAGESIZE ; i++ ) {
					this->data[i] = ( ( char* ) data ) [i];
				}
			}
			~Message()
			{

			}
			Message ( const Message& m ) :
				destiny ( m.destiny ), origin ( m.origin ),
				type ( m.type ), size ( m.size ), reach(m.reach)
			{
			     if(origin>2){
                    int a=2;
                }

				for ( int i = 0; i < MESSAGESIZE; i++ ) {
					this->data[i] = m.data[i];
				}
			}

			template<class T>
			Message (short destiny, short origin, short type,const T& datat, short reach = -1):
            destiny(destiny),origin(origin),type(type),reach(reach)
			{
			     if(origin>2){
                    int a=2;
                }

                size = sizeof(datat);
                char* temp = (char*)&datat;
                for (int i=0;i< sizeof(datat) && i< sizeof(data);i++){
                    data[i]= temp[i];
                }

			}
            template<class T>
            T getObject(){
                T* t =(T*) data;

                return *t;
            }

            template<class T>
            Message& operator>>(T& e){
                //T* t =(T*) data;
                e = *(T*)data;
                return *this;
            }


			void* getData()
			{
				return data;
			}
			short getDataSize()
			{
				return MESSAGESIZE;
			}

			short getDest()
			{
				return destiny;
			}

			short getOrig()
			{
				return origin;
			}

			short getTipo()
			{
				return type;
			}
			friend ofstream& operator<< ( ofstream& of, const Message& m )
			{
				of.sendBytes( &m , sizeof(m) );
				return of;
			}

			friend ifstream& operator>> ( ifstream& is, Message& m )
			{
			    int sizez= sizeof(Message);
				char buffer[sizez];

				is.readBytes( buffer, sizez);
				m = *(( Message* ) buffer);
                return is;
			}
			int getHash(){
                int first = ((origin & 255)<<24);
                int second = ((destiny & 255)<<16);
                int last = control;
                return (first + second + last);
			}
        private :



	};
// Message::controlCounter =0;

}

#endif
