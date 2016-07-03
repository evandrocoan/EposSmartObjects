
#ifndef OBSERVERPJ_H
#define OBSERVERPJ_H

#include "Listener.h"


namespace pj{

template<class Tipo>
class Observer
{
	public:
        Observer(int n=10);
        ~Observer(){
            delete(lista);
        }
		virtual int addListener ( Listener<Tipo>* listener );
		virtual int removeListener ( Listener<Tipo>* listener );
		virtual void clearListener ( );
		virtual bool hasListener ( Listener<Tipo>* listener );
		virtual void notifyListeners ( Tipo t );

	protected:
        Listener<Tipo>** lista;
        bool* occupied;
        int size;
	private:
};

template <typename T>
 Observer<T>::Observer(int n):size(n){
    lista = new Listener<T>*[n];
    occupied = new bool[n];

    for (int i=0;i<n;i++){
        occupied[i]=0;
    }
   // size=n;
}


template <typename T>
int Observer<T>::addListener ( Listener<T>* listener ) {
    int i;
    for ( i=0;i<size;i++){
        if(occupied[i]==0)break;
    }

    if(i==size)return -1;

    lista[i]= listener;
    occupied[i] =true;
    return 0;


}
template <typename T>
int Observer<T>::removeListener ( Listener<T>* listener ) {
    for (int i=0;i<size;i++){
        if(lista[i]==listener ){
            occupied[i] = false;
            return 0;
        }
    }
    return -1;
}
template <typename T>
void Observer<T>::clearListener ( ) {
    for (int i=0;i<size;i++){
        occupied[i]=0;
    }
}
template <typename T>
bool Observer<T>::hasListener ( Listener<T>* listener ) {
    for (int i=0;i<size;i++){
        if(lista[i]==listener ){
            return true;
        }
    }
    return false;
}
template <typename T>
void Observer<T>::notifyListeners ( T t ) {
    for (int i=0;i<size;i++){
        if(occupied[i]){
            lista[i]->notify( t);
        }
    }
}
}
#endif // OBSERVER_H
