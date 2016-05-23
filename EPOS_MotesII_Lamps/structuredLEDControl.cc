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
 * *****************************************************************************
 * 
 * Program notes
 * 
 * The code is not working using the default scheduler provided by the EPOSMoteII platform.
 * We are still deciding which scheduling algorithm and quantum to use, but for test purposes
 * we believe that it's okay to use Round_Robin. The problem is that while using the Round_Robin scheduler,
 * you get the feel that the LED is blinking, which should not be available to the user.
 * 
 * How to change the scheduler:
 * In the EPOSMoteII directory, edit the file include/traits.h
 * 
 * The scheduling algorithm should be located inside the templatized Traits<Thread> structure.
 * Also, you can change the quantum which each thread will be given by the scheduler.
 * 
 * pq tem os 3 coloridos ali
 * e tem um outro q fica do lado
 * qnd ele ta ligado eh pq deu merda
 * hmmm
 * ql escalonador vc ta usando?
 * vc mudou o escalonador?
 * 
 * vai na pasta include
 * do epos isso
 * vai em traits
 * vai ter um Traits<Thread>
 * la dentro, muda pra Round Robin
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

//TODO
//Get register addresses from include/mach/mc13224v/memory_map.h and implement function.
//Also implement method setReg.
//Please do not suicide.
/*
 void PWM1_Init(void)
 {
 setReg(TMR0_CNTR,0); // Reset counter
 /// TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
 //Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=1,OPS=0,OEN=1
 setReg(TMR0_SCTRL,0x05); // Enable output
 setReg(TMR0_COMP1,1500); // Store initial value to the duty-compare register
 // TMR0_CTRL: CM=1,PCS=8,SCS=0,ONCE=0,LENGTH=0,DIR=0,Co_INIT=0,OM=6
 setReg(TMR0_CTRL, 0x3006);
 }
 */

bool useSensor = false;

int defaultPower(int power) {
	return power;
}

int sensorCalculatePower(int power) {
	return 0;
}

/**
 *  Base IO adress. GPIO Base is the base address of all IO operations.
 *  copied from memory_map.h
 */
const unsigned int GPIO_BASE = 0x80000000;

/**
 * DATA_SET is just so magical I can't express my thoughts into words.
 */
const unsigned int GPIO_DATA_SET0 = GPIO_BASE + 0x48;

/**
 * PAD_DIR0 contains the base address which contains the registers' values.
 */
const unsigned int GPIO_PAD_DIR0 = GPIO_BASE + 0x00;

/**
 * MAX_MESSAGE_LENGTH_ALLOWED is the maximum length of the message which can be sent and received via UART/NIC
 */
const unsigned int MAX_MESSAGE_LENGTH_ALLOWED = 5;

/**
 * Only LEDs 0 to 2 (RGB) are used
 */
const unsigned int MAX_LEDS_ALLOWED_TO_BE_USED = 3;

/**
 * Please save yourself the trouble and don't read the documentation bellow. If you do
 * and still don't get it like me, then please increase the following counter to warn
 * future code revisor and save them the trouble. You have been warned.
 * count = 2
 * //4294967294 //4.294.967.294
 */
unsigned int g_effectDelay = 1e5;

/**
 * Used to stop all running threads which are running throw an while true, to perform busy wait
 * from this board devices.
 */
bool g_finishThread = false;

/**
 * Power of the lads.
 */
unsigned int power[MAX_LEDS_ALLOWED_TO_BE_USED]; // only leds 0 to 2 (RGB) are used

/**
 * Stdout for debuggging.
 */
OStream cout;

//Mutex* mutexEffect[MAX_LEDS_ALLOWED_TO_BE_USED];
bool g_effect[MAX_LEDS_ALLOWED_TO_BE_USED];

/**
 * Semaphore* semcout;
 */
NIC * g_nic;

/**
 * This function is used to toggle the led on or off.
 *
 * @param pin        The pin number that we want to toggle
 *
 * @param on         If this is true, we toggle the bit on. If this is false, we toggle the bit off
 *
 */
void turn_led(int pin, bool on) {
	int bit = pin % 32;
	unsigned int regData = GPIO_DATA_SET0 + ((pin >> 5) << 2);
	unsigned int regPad = GPIO_PAD_DIR0 + ((pin >> 5) << 2);
	unsigned int value = CPU::in32(regPad);

	if (on) {
		value |= (1 << bit);
	} else {
		value &= !(1 << bit);
	}

	CPU::out32(regPad, value);
	CPU::out32(regData, (1 << bit));
}

/**
 * This function executes the pulse width modulation for the LEDS in software.
 *
 * @return              I have no idea.
 */
/*
 int PWMLeds() {
 // semcout->p();
 cout << "Thread PWM LEDs initing\n";
 powerCalculateFunc func;
 // semcout->v();
 if (useSensor) {
 func = sensorCalculatePower;
 } else {
 func = defaultPower;
 }
 int led[5]; // not all leds are actually used. Only the RGB ones (the first 3)
 led[0] = 10;
 led[1] = 9;
 led[2] = 11;
 led[3] = 23;
 led[4] = 8;

 unsigned int i;
 unsigned int cont = 0;
 int calculatedPow[MAX_LEDS_ALLOWED_TO_BE_USED];
 for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
 power[i] = 50; // leds start at 1% of the power (just to show app is running)

 }

 // PWM
 while (!g_finishThread) {
 if (!cont) {
 for (int i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; ++i) {
 calculatedPow[i] = func(power[i]);
 }
 }
 //cout << "Still executing PWM. " << cont <<  "\n";
 cont == 99 ? cont = 0 : cont++;

 for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
 turn_led(led[i], cont < calculatedPow[i]);
 }

 // if (cont==0 && power[i]>0)
 //turn_led(led[i],true);
 //else
 //if (cont==power[i])
 //turn_led(led[i],false);

 }

 cout << "Thread PWM LEDs finishing\n";
 return 0;
 }
 */
void InterpretMessage(char msg[MAX_MESSAGE_LENGTH_ALLOWED]) {
	unsigned int led, pow, i;

	switch (msg[0]) {
	case 'R':
		led = 0;
		break;
	case 'G':
		led = 1;
		break;
	case 'B':
		led = 2;
		break;
	case 'A':
		led = MAX_LEDS_ALLOWED_TO_BE_USED;
		break;
	default:
		// semcout->p();
		cout << "Invalid led value '" << msg[0] << "' on position 0\n";
		// semcout->v();
		return;
	}

	//led = ((unsigned int)msg[0])-48; // int based on ascii
	if (msg[1] == 'E') {
		//g_effect
		if (msg[2] == 'N') // turn ON g_effect
				{
			if (led < MAX_LEDS_ALLOWED_TO_BE_USED) // one led
					{ // mutexEffect[led]->unlock();
				g_effect[led] = true;
			} else // all leds at once
			{
				for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
					// mutexEffect[i]->unlock();
					g_effect[i] = true;
				}
			}
			// semcout->p();

			cout << "Effect[" << led << "]=ON\n";
			// semcout->v();
		} else if (msg[2] == 'F') // turn OFF g_effect
				{
			if (led < MAX_LEDS_ALLOWED_TO_BE_USED) {
				// mutexEffect[led]->lock();
				g_effect[led] = false;
			} else {
				for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
					// mutexEffect[i]->lock();
					g_effect[i] = false;
				}
			}
			// semcout->p();
			cout << "Effect[" << led << "]=OFF\n";
			// semcout->v();
		} else   // set g_effect delay
		{
			unsigned int tempDelay = ((unsigned int) msg[2]) - 48;
			tempDelay *= 10 ^ (((unsigned int) msg[3]) - 48);
			g_effectDelay = tempDelay;
			// semcout->p();
			cout << "Delay=" << g_effectDelay << "\n";
			// semcout->v();
		}
	} else if (msg[1] == '0' || msg[1] == '1') {
		// fixed power
		pow = 100 * (((unsigned int) msg[1]) - 48);
		pow += 10 * (((unsigned int) msg[2]) - 48);
		pow += 1 * (((unsigned int) msg[3]) - 48);

		if (pow > 100) {
			pow = 100;
		}

		if (led < MAX_LEDS_ALLOWED_TO_BE_USED) // only one led
				{
			power[led] = pow;
		} else // all leds at once
		{
			for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
				power[i] = pow;
			}
		}
		// semcout->p();
		cout << "Power[" << led << "]=" << pow << "\n";
		// semcout->v();
	} else {
		// semcout->p();
		cout << "Invalid value '" << msg[1] << "' on position 1\n";
		// semcout->v();
	}
}

void SendMessageToNIC(char msg[MAX_MESSAGE_LENGTH_ALLOWED]) {
	int r;

	while ((r = g_nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof(msg)))
			== 0) // != 11 ?
	{ // semcout->p();
		cout << "Send failed " << r << "\n";
		// semcout->v();
	}

	// semcout->p();
	cout << "Message sent\n";

	// semcout->v();
}

int ReceiveCommandUART() {
	cout << "Thread UART initing\n";
	unsigned int i;
	char msg[MAX_MESSAGE_LENGTH_ALLOWED]; //[DATA_SIZE];
    
    cout << "To send commands to the EPOSMotes2 by USB device, use: \n";
    cout << "echo :R100 > /dev/ttyUSB0\n\n";
    cout << "Try also :REN, :BEN, :GEN or :AEN"
    cout << "\nAll commnds must to start with : (colon)\n";
    
	// semcout->p();

	// semcout->v();
	UART * uart = new UART();

	while (!g_finishThread) {
		do {
			msg[0] = uart->get();
		} while (msg[0] != ':'); // messages start with ":"
		i = 0;

		while ((msg[i - 1] != '\n') && (i < MAX_MESSAGE_LENGTH_ALLOWED)) {
			msg[i++] = uart->get();
		}
		memset(msg + i, 0x00, MAX_MESSAGE_LENGTH_ALLOWED - i);
		// message received.
		SendMessageToNIC(msg);
		InterpretMessage(msg);
	}

	cout << "Thread UART finishing\n";
	return 0;
}

int ReceiveCommandNIC() {
	// semcout->p();
	cout << "Thread NIC initing\n";
	// semcout->v();

	char msg[MAX_MESSAGE_LENGTH_ALLOWED];

	NIC::Protocol prot;
	NIC::Address src;

	while (!g_finishThread) {
		while (!(g_nic->receive(&src, &prot, &msg, sizeof(msg)) > 0)) {
			// semcout->p();
			//cout << ".";
			// semcout->v();
		}
		// semcout->p();
		cout << "\nMessage received: " << msg << "\n";
		// semcout->v();
		InterpretMessage(msg);
	}

	cout << "Thread NIC finishing\n";
	return 0;
}

int LEDPowerEffect() {
	// semcout->p();
	cout << "Thread Effect initing\n";
	// semcout->v();
	//unsigned int i = MAX_LEDS_ALLOWED_TO_BE_USED;
	unsigned int j;
	int pow;

	while (!g_finishThread) {
		//for (i=0; i<=MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
		// semcout->p();

		// semcout->v();
		for (pow = 0; pow <= 100; pow++) {
			for (int i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; ++i) {
				// mutexEffect[i]->lock();
				//if (i<MAX_LEDS_ALLOWED_TO_BE_USED) // only one led
				if (g_effect[i]) {
					power[i] = pow;

					//else // all leds at once
					//   for (j=0; j<MAX_LEDS_ALLOWED_TO_BE_USED; j++)
					//      power[j]=pow;
					// mutexEffect[i]->unlock();
					Alarm::delay(g_effectDelay);
				}
			}
		}
		for (pow = 100; pow >= 0; pow--) {
			for (int i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; ++i) {
				if (g_effect[i]) {

					// mutexEffect[i]->lock();
					//if (i<MAX_LEDS_ALLOWED_TO_BE_USED)
					power[i] = pow;
					//else
					//   for (j=0; j<MAX_LEDS_ALLOWED_TO_BE_USED; j++)
					//      power[j]=pow;
					// mutexEffect[i]->unlock();
					Alarm::delay(g_effectDelay);
				}
			}
		}

	}

	cout << "Thread Effect finishing\n";
	return 0;
}

void PWMInterrupt() {
	static int count = 0;
	static bool turnOn = false;
	int led[5]; // not all leds are actually used. Only the RGB ones (the first 3)
	led[0] = 10;
	led[1] = 9;
	led[2] = 11;
	led[3] = 23;
	led[4] = 8;
	for (int i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; ++i) {
		if (count < power[i]) {
			turn_led(led[i], true);
		} else {
			turn_led(led[i], false);
		}
	}
	count = (count + 1) % 100;
}

int main() {
	cout << "EposMotesII app initing\n";
	unsigned int i;
	TSC_Timer pwmTimer(100, &PWMInterrupt);
//Alarm::delay(100);
	for (i = 0; i < MAX_LEDS_ALLOWED_TO_BE_USED; i++) {
		//   mutexEffect[i]= new Mutex();
		//   mutexEffect[i]->lock(); // g_effect starts OFF (blocked)
		g_effect[i] = false;
	}
	g_nic = new NIC();
// semcout = new Semaphore(1);

	Thread * thrdPWM;
	Thread * thrdUART;
	Thread * thrdNIC;
	Thread * thrdEffect;
//Uncomment later when use photo sensor.
//useSensor = myCuteSensor.enable();

	thrdUART = new Thread(&ReceiveCommandUART);

//thrdNIC  = new Thread(&ReceiveCommandNIC);
	thrdEffect = new Thread(&LEDPowerEffect);
	Alarm::delay(5e6);

// semcout->p();
	cout << "Waiting for threads to finish\n";
// semcout->v();

	int status_thrdUART = thrdUART->join();

//int status_thrdNIC  = thrdNIC->join();
	int status_thrdEffect = thrdEffect->join();
	cout << "Threads finished. EposMotesII app finishing\n";
//Lista das pessoas que se importam com essa parte do código:

//Fim da lista
//thrdPWM = new Thread(&PWMLeds);
//int status_thrdPWM = thrdPWM->join();
	return 0;
}
