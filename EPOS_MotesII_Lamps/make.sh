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

# Switch to the main/compilation directory.
cd $EPOS

# To clear any last compilation data.
if [[ $2 == *"clean"* ]]
then
    make veryclean all
fi

# To compile the application passed as parameter '$programNameToCompile'
make APPLICATION=$programNameToCompile

# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP




