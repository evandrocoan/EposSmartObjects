/**
 * Structured LED EposMotesII Controlling
 *
 * This work is licensed under the EPOS Software License v1.0.
 * A copy of this license is available at the EPOS system source tree root.
 * Note that EPOS Software License applies to both source code and executables.
 *
 *  You should have received a copy of the EPOS Software License v1.0 along
 *  with this program. If not, see
 *  <http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0>.
 *
 */

#include <machine.h>
#include <alarm.h>
#include <sensor.h>
#include <battery.h>
#include <uart.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <traits.h>

__USING_SYS

#include "tinclude.h"
int main() {
	cout << "EposMotesII app initing\n";
	unsigned int i;
	Timer myFavoriteTimer(100, myCuteHandler);
	Alarm::delay(100);

	for (i = 0; i < MAX_LEDS; i++) {
		//   mutexEffect[i]= new Mutex();
		//   mutexEffect[i]->lock(); // effect starts OFF (blocked)
		effect[i] = true;
	}

	nic = new NIC();
	// semcout = new Semaphore(1);

	Thread * thrdPWM;
	Thread * thrdUART;
	Thread * thrdNIC;
	Thread * thrdEffect[MAX_LEDS];
	//Uncomment later when use photo sensor.
	//useSensor = myCuteSensor.enable();

	cout << "About to create UART thread\n";
	thrdUART = new Thread(&ReceiveCommandUART);
	cout << "Created UART thread\n";

	//thrdNIC  = new Thread(&ReceiveCommandNIC);
	for (i = 0; i < MAX_LEDS; i++) {
		cout << "Creating thread effect " << i << "\n";
		thrdEffect[i] = new Thread(&LEDPowerEffect, (unsigned int) i);
		cout
				<< "About to sleep for a loooooooooooooooooooooooooooooooooooooooooong time\n";
		Alarm::delay(5e6);
		cout << "This mother fucker woke up\n";
	}

	// semcout->p();
	cout << "Waiting for threads to finish\n";
	// semcout->v();

	int status_thrdUART = thrdUART->join();

	//int status_thrdNIC  = thrdNIC->join();
	for (i = 0; i < MAX_LEDS; i++) {
		int status_thrdEffect = thrdEffect[i]->join();
	}
	cout << "Threads finished. EposMotesII app finishing\n";
	//Lista das pessoas que se importam com essa parte do código:



	//Fim da lista
	//thrdPWM = new Thread(&PWMLeds);
	//int status_thrdPWM = thrdPWM->join();
	return 0;
}
