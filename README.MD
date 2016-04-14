#EposMotesII Smart Objects



#To contribute

1. First, to install EPOS 1.1 for the "EPOSMote II" device, download these files:
  * [File: "OpenEPOS-1.1RC.tgz" - OpenEPOS-1.1 RC tree snapshot from October 18, 2011](http://epos.lisha.ufsc.br/dl95)
  * [File: "arm-gcc-4.4.4.tar.gz" - ARM (ARM7 and others)](http://epos.lisha.ufsc.br/dl88)
1. To install EPOS 2.0 for the "EPOSMote III" device, visit the LISHA official Website:
  * [EPOS 2.0 User's Guide](http://epos.lisha.ufsc.br/EPOS+2+User+Guide)
  * [Download Page](http://epos.lisha.ufsc.br/EPOS+Software)
    * Observation: You need to register/create a free account, in order to be able to see the EPOS 1.1 
    or 2.0 manuals.
    * See also the [EPOS 1.1 User's Guide](http://epos.lisha.ufsc.br/EPOS+User+Guide).

In order to work with EPOS, you need:

1. To extract the contents of "OpenEPOS-1.1RC.tgz" file where you would like most.
1. To set some environment variables (on the user's bash config file "home/User/.bashrc"):
  * export EPOS=/path/to/epos
  * export PATH=$PATH:$EPOS/bin
1. To clone this repository inside:
  * $EPOS/app/
    * Example: home/User/Desktop/OpenEPOS-1.1RC/app/INE5412_EposMotesII_SmartObjects
1. To extract the contents of "arm-gcc-4.4.4.tar" file at the directory:
  * /usr/local/arm/
    * Observation: Rename the folder "gcc-4.4.4", inside "arm-gcc-4.4.4.tar" to "gcc".
  * For instance, if you downloaded the ia32 toolchain, you should extract it at /usr/local/ia32/
    * If you do not have access to that path, you'll have to adjust the makedefs file in EPOS' 
    main directory accordingly.
1. You don't need to bother about the chosen path nor set any environment variable. EPOS is fully 
self contained.



#Creating an EPOS Application

Create a new file in $EPOS/app:
* $EPOS/app/INE5412_EposMotesII_SmartObjects/helloworld.cc

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


The system will be configured and compiled (i.e. generated) successive times for each 
application found in the app directory. Both software and hardware components will be 
generated according with each application needs and stored in the img directory.



#Building an EPOS Application
If you have multiple applications or multiple deployment scenarios, but want to operate 
on a single one, you can specify it using the APPLICATION parameter like this:
* $ make APPLICATION=INE5412_EposMotesII_SmartObjects/helloworld
  * Observation: You must to compile EPOS with "make" as explained above before to compile one 
  single APPLICATION.

If everything goes right, you should end with something like this:
```cpp
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
