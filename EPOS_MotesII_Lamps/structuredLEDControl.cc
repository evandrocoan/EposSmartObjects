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


/**
 * Change this typedef to change the type of the element for the bit vector.
 * Rule is for it to be small enough to waste less memory;
 */
typedef unsigned char bitarr_el;

/**
 * Abstraction provided to access a bitarray. Caution: do not access it with []
 */
typedef bitarr_el* bitarr_t;

/**
 * Works like a C++ constructor.
 * @param arrSize		The size of the array
 * @return				A pointer to the array.
 */
bitarr_t createBitArray(size_t arrSize) {
	bitarr_t ar;
	ar = new bitarr_el[arrSize/sizeof(bitarr_el) + 1];
	return ar;
}

/**
 * Frees memory from bit array;
 * @param arr			Memory to be freed
 */
void destroyBitArray(bitarr_t arr) {
	delete[] arr;
}

/**
 * Checks to see if the bit at the given position is set
 * @param arr			The bit array
 * @param pos			The position
 * @return				true if bit is 1 else false
 */
bool checkPosition(bitarr_t arr, size_t pos) {
	//Calculate region
	size_t region = pos/sizeof(bitarr_el);
	//Get element at region
	bitarr_el* element = arr + region;
	//Calculate index inside element
	size_t elementPos = pos % sizeof(bitarr_el);
	//Check bit of element
	return (*element) & (1 << (elementPos));
}
/**
 * Sets bit on given position of the array
 * @param arr			The bit array
 * @param pos			The position in the array
 * @param value			If value is true, sets bit to 1 else 0.
 */
void setPosition(bitarr_t arr, size_t pos, bool value) {
	//Calculate region
	size_t region = pos/sizeof(bitarr_el);
	//Get element at region
	bitarr_el* element = arr + region;
	//Calculate index inside element
	size_t elementPos = pos % sizeof(bitarr_el);
	if(value) {
		*element |= (1 << elementPos);
	}
	else {
		*element &= !(1 << elementPos);
	}
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
 * MSG_LEN is the maximum length of the message which can be sent and received via UART/NIC
 */
const unsigned int MSG_LEN = 5;

/**
 *  only leds 0 to 2 (RGB) are used
 */
const unsigned int MAX_LEDS = 3;

/**
 * Please save yourself the trouble and don't read the documentation bellow. If you do
 * and still don't get it like me, then please increase the following counter to warn
 * future code revisor and save them the trouble. You have been warned.
 * count = 2
 * //4294967294 //4.294.967.294
 */
unsigned int effectDelay = 1e5;

bool finishThread = false;

/**
 * Power of the lads.
 */
unsigned int power[MAX_LEDS]; // only leds 0 to 2 (RGB) are used

/**
 * Stdout for debug reasons.
 */
OStream cout;

bool effect[MAX_LEDS];

NIC * nic;
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

void InterpretMessage(char msg[MSG_LEN]) {
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
		led = MAX_LEDS;
		break;
	default:
		cout << "Invalid led value '" << msg[0] << "' on position 0\n";
		return;
	}

	if (msg[1] == 'E') {
		//effect
		if (msg[2] == 'N') // turn ON effect
				{
			if (led < MAX_LEDS) // one led
					{
				effect[led] = true;
			} else // all leds at once
			{
				for (i = 0; i < MAX_LEDS; i++) {
					effect[i] = true;
				}
			}

			cout << "Effect[" << led << "]=ON\n";
		} else if (msg[2] == 'F') // turn OFF effect
				{
			if (led < MAX_LEDS) {
				effect[led] = false;
			} else {
				for (i = 0; i < MAX_LEDS; i++) {
					effect[i] = false;
				}
			}
			cout << "Effect[" << led << "]=OFF\n";
		} else   // set effect delay
		{
			unsigned int tempDelay = ((unsigned int) msg[2]) - 48;
			tempDelay *= 10 ^ (((unsigned int) msg[3]) - 48);
			effectDelay = tempDelay;
			cout << "Delay=" << effectDelay << "\n";
		}
	} else if (msg[1] == '0' || msg[1] == '1') {
		// fixed power
		pow = 100 * (((unsigned int) msg[1]) - 48);
		pow += 10 * (((unsigned int) msg[2]) - 48);
		pow += 1 * (((unsigned int) msg[3]) - 48);

		if (pow > 100) {
			pow = 100;
		}

		if (led < MAX_LEDS) // only one led
				{
			power[led] = pow;
		} else // all leds at once
		{
			for (i = 0; i < MAX_LEDS; i++) {
				power[i] = pow;
			}
		}
		cout << "Power[" << led << "]=" << pow << "\n";
	} else {
		cout << "Invalid value '" << msg[1] << "' on position 1\n";
	}
}

void SendMessageToNIC(char msg[MSG_LEN]) {
	int r;

	while ((r = nic->send(NIC::BROADCAST, (NIC::Protocol) 1, &msg, sizeof(msg)))
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
	char msg[MSG_LEN]; //[DATA_SIZE];

	// semcout->p();

	// semcout->v();
	UART * uart = new UART();

	while (!finishThread) {
		do {
			msg[0] = uart->get();
		} while (msg[0] != ':'); // messages start with ":"
		i = 0;

		while ((msg[i - 1] != '\n') && (i < MSG_LEN)) {
			msg[i++] = uart->get();
		}
		memset(msg + i, 0x00, MSG_LEN - i);
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

	char msg[MSG_LEN];

	NIC::Protocol prot;
	NIC::Address src;

	while (!finishThread) {
		while (!(nic->receive(&src, &prot, &msg, sizeof(msg)) > 0)) {
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
	//unsigned int i = MAX_LEDS;
	unsigned int j;
	int pow;

	while (!finishThread) {
		for (pow = 0; pow <= 100; pow++) {
			for (int i = 0; i < MAX_LEDS; ++i) {
				if (effect[i]) {
					power[i] = pow;
					Alarm::delay(effectDelay);
				}
			}
		}
		for (pow = 100; pow >= 0; pow--) {
			for (int i = 0; i < MAX_LEDS; ++i) {
				if (effect[i]) {
					power[i] = pow;
					Alarm::delay(effectDelay);
				}
			}
		}

	}

	cout << "Thread Effect finishing\n";
	return 0;
}

void PWMInterrupt() {
	static int count = 0;
	int led[5]; // not all leds are actually used. Only the RGB ones (the first 3)
	led[0] = 10;
	led[1] = 9;
	led[2] = 11;
	led[3] = 23;
	led[4] = 8;
	for (int i = 0; i < MAX_LEDS; ++i) {
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
	for (i = 0; i < MAX_LEDS; i++) {
		effect[i] = false;
	}
	nic = new NIC();

	Thread * thrdPWM;
	Thread * thrdUART;
	Thread * thrdNIC;
	Thread * thrdEffect;

	thrdUART = new Thread(&ReceiveCommandUART);

//thrdNIC  = new Thread(&ReceiveCommandNIC);
	thrdEffect = new Thread(&LEDPowerEffect);
	Alarm::delay(5e6);

	cout << "Waiting for threads to finish\n";

	int status_thrdUART = thrdUART->join();

	int status_thrdEffect = thrdEffect->join();
	cout << "Threads finished. EposMotesII app finishing\n";
	return 0;
}
