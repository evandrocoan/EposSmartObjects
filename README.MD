
#Controle integrado e distribuído de luminárias

Os requisitos funcionais do sistema poderiam ser os seguintes:

- Permitir que a luminária possa cadastrar-se, informando seu ID e os serviços que ela executa (controle de intensidade de cada cor, etc) a um servidor, e saber quando ela já foi cadastrada nesse servidor. Isso deve permitir que posteriormente ela receba mensagens com comandos.

- Através de mensagens recebidas, controlar a cor e a intensidade do brilho de uma luminária de LEDs RGB (usando PWM em hardware), fazendo essas intensidades fixas ou mudando com o tempo

- Através de mensagens recebidas, cadastrar uma configuração de cores e intensidades para cada possível usuário, e aplicar essa configuração quando um usuário está presente (informação recebida via mensagem)

- Permitir que se a mensagem de comando recebida não for para esta luminária, reencaminhar a mensagem para outros objetos inteligentes


Como requisitos não-funcionais, acho que tempos, principalmente:

- Ser implementado com EPOS 1.1 na placa EposMotesII


Acho que vocês precisarão de 3 EposMotesII: Dois para controlar luminárias diferentes e um para funcionar como gateway ou para indicar a presença de uma pessoa.


#Para contribuir

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
```
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
