#ifndef MENSAGEM

#include "ofstream.h"
#include "ifstream.h"

#define MESSAGESIZE 40


namespace pj
{

	class Message
	{

		private:
			short destiny;
			short origin;
			short type;
			short size;
            short reach;
			char data[MESSAGESIZE];

		public:
			Message()
			{
			}



			Message ( short destino, short origem, short type, void* data, short size, short reach = -1 )
				:  destiny ( destino ), origin ( origem ), type ( type ), size ( size )
			{

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
				for ( int i = 0; i < size; i++ ) {
					this->data[i] = m.data[i];
				}
			}

			template<class T>
			Message (short destiny, short origin, short type,const T& datat, short reach = -1):
            destiny(destiny),origin(origin),type(type),reach(reach)
			{
                size = sizeof(datat);
                char* temp = (char*)&datat;
                for (int i=0;i< sizeof(datat) && i< sizeof(data);i++){
                    data[i]= temp[i];
                }

			}

            template<class T>
            T getData(){
                T* t =(T*) data;
                return *t;
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
			    /*
                int bufferSize = sizeof(m.destiny) + sizeof(m.origin) + sizeof(m.type) + sizeof(m.size) + sizeof(m.reach);
                bufferSize += m.size;

                char buffer[bufferSize];
                char* ini = buffer;

                *ini = m.destiny;
                ini+= sizeof(m.destiny);

                *ini = m.origin;
                ini+= sizeof()
                */


				of.sendBytes( &m , sizeof(m) );


				return of;
			}

			friend ifstream& operator>> ( ifstream& is, Message& m )
			{
				char buffer[sizeof ( Message )];
				is >> buffer;
				m = *(( Message* ) buffer);

			}


	};

}

#endif
