
// This work is licensed under the EPOS Software License v1.0.
// A copy of this license is available at the EPOS system source tree root.
// A copy of this license is also available online at:
// http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
// Note that EPOS Software License applies to both source code and executables.
//#include <system/memory_map.h>

#include <traits.h>
__USING_SYS

unsigned int *GPIO_BASE = (unsigned int*)0x80000000;

bool led1=false;
bool led2=false;

 static void wait(unsigned int us) {
        for (unsigned int i = 0; i <= us * (Traits<Machine>::CLOCK / 1000000UL); i++); 
    }


void toggle_on_led1() {
	if(!led1){
		*GPIO_BASE = *GPIO_BASE | (1 << 23);
		led1=true;
	}else{
		*GPIO_BASE = *GPIO_BASE & !(1 << 23);
		led1=false;
	}
}

void toggle_on_led2() {
    if(!led2){
		*GPIO_BASE = *GPIO_BASE | (1 << 24);
		led2=true;
	}else{
		*GPIO_BASE = *GPIO_BASE & !(1 << 24);
		led2=false;
	}
}



int main() {
  toggle_on_led1();
  toggle_on_led2();
  wait(2000);
  toggle_on_led1();
  return 0;

 
}

