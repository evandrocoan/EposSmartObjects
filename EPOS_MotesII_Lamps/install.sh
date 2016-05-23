#!/bin/sh

installManual=$(cat "_installManual.txt")

# Print help to the output stream.
printHelp()
{
    echo "The start directory is $PWD_COMPILE_EPOS_LAMP"
    echo "The current directory is $EPOS"
    echo "$installManual"
}


# Saves the current opened path, to restore it when this scripts finish.
PWD_COMPILE_EPOS_LAMP=$(pwd)

# The EPOSMotes2 installer
EPOS_MOTES2_INSTALLER="red-bsl.py"
EPOS_MOTES2_INSTALLER_BINARY1="flasher.bin"
EPOS_MOTES2_INSTALLER_BINARY2="ssl.bin"

# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
    || [ $# -eq 0 ]
then
    echo "\nERROR! Could not find $PWD_COMPILE_EPOS_LAMP/$programFileToCompile"
    printHelp
    exit 1
fi

if [ $# -eq 0 ]
then
    echo "\nERROR! Bad USB port number!"
    echo "To run it, use: sudo cutecom /dev/ttyUSB<number>"
    echo "Example: sudo cutecom /dev/ttyUSB0 &"
    exit 1
fi



# Switch to the main/compilation directory.
cd $EPOS

# To copy the the compiler file 'red-bsl.py' to be used.
if ! [ -f $EPOS_MOTES2_INSTALLER ] \
    || ! [ -f $EPOS_MOTES2_INSTALLER_BINARY1 ] \
    || ! [ -f $EPOS_MOTES2_INSTALLER_BINARY2 ]
then
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER $EPOS/
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER_BINARY1 $EPOS/
    cp $EPOS/tools/emote/$EPOS_MOTES2_INSTALLER_BINARY2 $EPOS/
fi


echo "The start directory is $PWD_COMPILE_EPOS_LAMP"
echo "The current directory is $EPOS"

# To send the compiled application to the EPOSMotes2 board.
# python red-bsl.py -t /dev/ttyUSB0 -f img/structuredLEDControl.bin -S
sudo python $EPOS_MOTES2_INSTALLER -t /dev/ttyUSB0 -f img/$programNameToCompile.bin -S


# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

echo "ATTENTION! Install cutecom to read the EPOSMotes2 cout stream output."
echo "To install it, use: sudo apt-get install cutecom"
echo "To run it, use: sudo cutecom /dev/ttyUSB<number> &"
echo "Example: sudo cutecom /dev/ttyUSB0 &"
echo "\nAfter open cutemcom, click on the open button"
echo "then press the EPOSMotes2 reset button, otherwise"
echo "it will not work, to send commands to the EPOSMotes2"
echo "by USB device. As: echo :R100 > /dev/ttyUSB0\n"



