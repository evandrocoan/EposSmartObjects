#Integrated Control and distributed lamps.



The functional requirements of the system could be the following:
- Allow the lamp can subscribe by entering its ID and the services it performs (intensity control of each color, etc.) to a server, and know when it has already been registered on this server. This should allow it to subsequently receive messages with commands.
- Through incoming messages, control the color and the brightness of a lamp LED RGB (using PWM hardware), making these fixed intensities or changing with time
- Through incoming messages, registering a color setting and intensities for each possible user and apply this setting when a user is present (information received via message)
- Allow the received command message is not for this light, forward the message to other smart objects

As a non-functional requirements, I think times, mainly:
- To be implemented with EPOS 1.1 on board EposMotesII

I think you need 3 EposMotesII: Two different fixtures to control and to act as a gateway or to indicate the presence of a person.



#Scheduling
The first step is to get the given codes by the teacher, working fully right. The some of activities
can be performed at the same time. Next steps to get the MotesII device set up:

1. * 1.1) Control the color and the brightness of a lamp LED RGB, making these locked
    intensities or changing with time.
   * 1.2) Make the PWM hardware to be fully functional.

1. * 2.1) To allow the lamp (EposMotesII) to receive messages/commands by USB bus.
   * 2.2) To allow the lamp (EposMotesII) to send/receive messages by the radio board to others lamps devices.

2. * 3.1) Regularly searching for a new server to register and control itself (the lamp).
   * 3.2) To create a lamp server to register and control and lamps through messages.

3. * 4.1) Allow if the received command message is not for this light, forward the message to
    other smart objects.
   * 4.2) Through incoming messages, registering a color setting and intensities for each possible
    user and apply this setting when a user is present (information received via message).

4. * 5.1) Allow the lamp to subscribe to a server by entering its ID and the services it performs
    (intensity control of each color, etc.)
   * 5.2) Allow the lamp to know when it has already been registered on this server. This should
    allow it to subsequently receive messages with commands.

5. * 6.1) Create one device (another EposMotesII device) to indicate the presence of a person.
   * 6.2) To allow the lamps to detect the person presence device proximity.


Glossary:

1. A server is another EposMotesII which connected by USB to a computer, receives commands to
    control its registered lamps (another EposMotesII devices).
2. A person presence indicator is another EposMotesII which communicates with the lamps
    (another EposMotesII).



#To contribute

To install EPOS for the "EPOSMote II" device, download:
* [OpenEPOS-1.1 RC tree snapshot from October 18, 2011](https://github.com/evandrocoan/INE5412_EposMotesII_Final_Project/archive/master.zip)
* [ARM (ARM7 and others)](http://epos.lisha.ufsc.br/dl88)


In order to work with EPOS, you need to extract the contents of "arm-gcc-4.4.4.tar" file and set some environment variables:
* export EPOS=/path/to/epos (the 'OpenEPOS-1.1RC' folder on this project, on your local computer).
* export PATH=$PATH:$EPOS/bin


You don't need to bother about the chosen path nor set any environment variable. EPOS is fully self contained.

If you also downloaded a toolchain tarball (arm-gcc-4.4.4.tar), open it at /usr/local/<architecture> whenever possible. For instance, if you downloaded the ia32 toolchain, you should extract it at /usr/local/ia32/gcc-4.4.4. 
If you downloaded the arm toolchain for EPOSMote III, you should extract it at /usr/local/arm/gcc-4.4.4

If you do not have access to that path, you'll have to adjust the makedefs file in EPOS' main directory accordingly. See also the EPOS [Download Page](http://epos.lisha.ufsc.br/EPOS+Software) and [Installing User Guide](http://epos.lisha.ufsc.br/EPOS+User+Guide#Installing)



#Creating an EPOS Application

Create a new file in $EPOS/app:
* $EPOS/app/helloworld.cc

```cpp
#include <utility/ostream.h>

__USING_SYS

OStream cout;

int main() {
    cout << "\n Hello World! \n";
    return 0;
}
```



#Compiling EPOS

At the directory where you installed EPOS' source code, just type:

* $ make


The system will be configured and compiled (i.e. generated) successive times for each application found in the app directory. Both software and hardware components will be generated according with each application needs and stored in the img directory.



#Building an EPOS Application
If you have multiple applications or multiple deployment scenarios, but want to operate on a single one, you can specify it using the APPLICATION parameter like this:

* $ make APPLICATION=helloworld


If everything goes right, you should end with something like this:
```
EPOS bootable image tool

  EPOS mode: library
  Machine: cortex_m
  Model: emote3
  Processor: armv7 (32 bits, little-endian)
  Memory: 31 KBytes
  Boot Length: 0 - 0 (min - max) KBytes
  Node id: will get from the network

  Creating EPOS bootable image in "hello.img":
    Adding application "hello": done.

  Adding specific boot features of "cortex_m": done.

  Image successfully generated (52686 bytes)!
  ```



#Running

First of all, you'll need to install a platform specific back-end for
EPOS to run on. During development, this is usually a
[QEMU virtual machine](http://www.qemu.org/) for your target architecture
(e.g. qemu-system-i386, qemu-system-arm). Then, simply type:

* $ make [APPLICATION=<application>] run
