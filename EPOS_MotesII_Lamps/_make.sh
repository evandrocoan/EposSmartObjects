#!/bin/sh


# Saves the current opened path, to restore it when this scripts finish.
installManual=$(cat "_installManual.txt")
PWD_COMPILE_EPOS_LAMP=$(pwd)


# Print help to the output stream.
printHelp()
{
    echo "The start directory is $PWD_COMPILE_EPOS_LAMP"
    echo "The current directory is $EPOS"
    echo "$installManual"
}

# contains(string, substring)
#
# Returns 0 if the specified string contains the specified substring,
# otherwise returns 1.
contains()
{
    string="$1"
    substring="$2"
    
    if test "${string#*$substring}" != "$string"
    then
        return 0    # $substring is in $string
    else
        return 1    # $substring is not in $string
    fi
}


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
if contains $2 "clean"
then
    make veryclean all
else
    make
fi


# To compile the application passed as parameter '$programNameToCompile'
make APPLICATION=$programNameToCompile
printHelp

# Switch back to the start command line folder.
cd $PWD_COMPILE_EPOS_LAMP




