#include <iostream>
#include <pthread.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;

typedef unsigned short bit_array;

typedef struct {
	int address;
	char msg[16];
} msg_struct;

msg_struct* msg_space = (msg_struct *)malloc(sizeof(msg_struct));

using namespace std;

int setbit(int bit, int *array){
	
	int pos = bit %32;
	*array |= (1<<pos);
	return 1;
}

int unsetbit(int bit, int *array){
	int pos = bit %32;
	*array &= ~(1<<pos);
	return 1;
}

void* sendStuff(void* useless){
	int dst;
	setbit(1, &dst);
	cout << dst << endl;
	
	msg_space -> address = dst;
	strcpy (msg_space->msg,"Hello!!");
	
	//pthread_mutex_lock(&mutex);
	cout << "Sender sends to: " << msg_space -> address << " this " << msg_space->msg << endl;
	//pthread_mutex_unlock(&mutex);
	
	return NULL;
	
}

void* receiveStuff(void* id){
	
	int myId = *(int *) id;

	//pthread_mutex_lock(&mutex);
	cout<< myId << " what? " << msg_space->address << endl;
	//cout << ((1<<*myId) & msg_space->address) << endl;
	sleep(1);
	if(((msg_space->address) & (1<< myId)) > 0){
		//pthread_mutex_lock(&mutex);
		cout<< myId << " gotcha!! You said: " << msg_space->msg << endl;
		//pthread_mutex_unlock(&mutex);
	}
	
	
	
	return NULL;
}


int main(){
	
	unsigned int ID;
	unsigned int ID1;
	
	pthread_mutex_init(&mutex,NULL);
	
	pthread_t sender1, receiver1,receiver2;
	
	pthread_create(&sender1,NULL, sendStuff,NULL);
	
	sleep(1);
	ID = 0;
	pthread_create(&receiver1,NULL, receiveStuff,&ID);
	
	ID1 = 1;
	pthread_create(&receiver2,NULL, receiveStuff, &ID1);
	
	pthread_join(sender1,NULL);
	pthread_join(receiver1,NULL);
	pthread_join(receiver2,NULL);
	
	return 0;
	
	
}