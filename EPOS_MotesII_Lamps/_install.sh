#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
installManual=$(cat "__installManual.txt")
PWD_COMPILE_EPOS_LAMP=$(pwd)


# Print help to the output stream.
printHelp()
{
    printf "$installManual\n"
}

# Determine whether the first parameter is an integer or not.
#
# Returns 1 if the specified string is an integer, otherwise returns 0.
isInteger()
{
    if [ "$1" -eq "$1" ] 2>/dev/null
    then
        return 1
    else
        return 0
    fi
}


# The EPOSMotes2 installer
EPOS_MOTES2_INSTALLER="red-bsl.py"
EPOS_MOTES2_INSTALLER_BINARY1="flasher.bin"
EPOS_MOTES2_INSTALLER_BINARY2="ssl.bin"


# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
computerUSBNumber=$2

# Removed the file extension, just in case there exists.
programNameToCompile=$(echo $programFileToCompile | cut -d'.' -f 1)


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ] \
    || [ $# -eq 0 ]
then
    printf "\nINSTALL ERROR:\n Could not find $PWD_COMPILE_EPOS_LAMP/$programFileToCompile\n"
    printHelp
    exit 1
fi

if isInteger $computerUSBNumber
then
    printf "\nINSTALL ERROR:\n Bad USB port number $computerUSBNumber!\n"
    printf "Use: ./install.sh MY_COOL_PROGRAM_NAME_WITHOUT_HYPHEN.cc 0\n"
    exit 1
else
    computerUSBNumber=0
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


# To send the compiled application to the EPOSMotes2 board.
# python red-bsl.py -t /dev/ttyUSB0 -f img/structuredLEDControl.bin -S
if sudo python $EPOS_MOTES2_INSTALLER -t /dev/ttyUSB$computerUSBNumber -f img/$programNameToCompile.bin -S
then
    printf "ATTENTION: Install/use cutecom to read the EPOSMotes2 cout stream output.\n"
    printf "To install it, use: sudo apt-get install cutecom\n"
    printf "To run it, use: sudo cutecom /dev/ttyUSB<number> &\n"
    printf "Example: sudo cutecom /dev/ttyUSB0 &\n"
    printf "After open cutecom, click on the open button then press the EPOSMotes2 reset\n"
    printf "button, otherwise it will not work, to send commands to the EPOSMotes2\n"
    printf "by USB device. As: echo :R100 > /dev/ttyUSB0\n"
fi

# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP

# Print help when it is not passed a third command line argument integer
if isInteger $3
then
    printHelp
fi



