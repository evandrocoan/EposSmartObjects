PWD_COMPILE_EPOS_LAMP=$(pwd)
cp $EPOS/app/INE5412_EposMotesII_SmartObjects/EPOS_MotesII_Lamps/structuredLEDControl.cc $EPOS/app
cd $EPOS ; make veryclean all; make APPLICATION=structuredLEDControl ;  arm-objcopy -I elf32-little -O binary img/structuredLEDControl.img img/structuredLEDControl.bin
rm $EPOS/app/structuredLEDControl.cc
cd $PWD_COMPILE_EPOS_LAMP
unset PWD_COMPILE_EPOS_LAMP