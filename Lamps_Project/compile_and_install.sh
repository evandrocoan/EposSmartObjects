#!/bin/sh

# Saves the current opened path, to restore it when this scripts finish.
PWD_COMPILE_EPOS_LAMP=$(dirname $(readlink -f $0))

# Import the helper functions.
. ./installer_files/__helper_functions.sh


COMPILE_AND_INSTALL_VERSION="1.1.4"
# Change log
# 
# v1.1.4
# Added to the '_install.sh' script the option 'notsend' to allow skipt the last
# './compile_and_install.sh' script part.
# 
# v1.1.3
# Added to the '_copy.sh' script to force the make re-compile the project files when the main file
# is not changed.
# 
# v1.1.2
# Added to the '_copy.sh' script to copy the '.cc' files to the same location as the '.h' files.
# 
# v1.1.1
# Placed the compiler files into the folder 'installer_files'.
# 
# v1.1
# Placed the non-executable files into the folder 'files'.
# Fixed the 'showTheElapsedSeconds' not showing correctly the time.
# 
# v1.0
# Initial release.
# 

printf "Running the script './compile_and_install.sh' version $COMPILE_AND_INSTALL_VERSION...\n"


# Print help to the output stream.
printHelp()
{
    printf "\nCOMPILE AND INSTALL v$COMPILE_AND_INSTALL_VERSION HELP :\n"
    printf "The './compile_and_install.sh' parameters are:\n"
    printf "1) The FIRST parameter is the file name to install.\n"
    printf "2) The SECOND parameter is the USB port number to install, or the keyword 'notsend' \n"
    printf "   to skip the last part and do not send the application to the EPOSMotes2 board.\n"
    printf "3) The THIRD parameter is the optional value clean for a 'make veryclean' all.\n\n"
    printf "Examples:\n"
    printf "./compile_and_install.sh PROGRAM_NAME.cc 0 clean\n"
    printf "./compile_and_install.sh PROGRAM_NAME.cc notsend\n"
    printf "./compile_and_install.sh PROGRAM_NAME.cc 1\n"
}


# Read the command line argument. The programs name must to be without type extension.
programFileToCompile=$1
usbPortNumberToInstall=$2
isVeryCleanCompilation=$3



# Calculates whether the $isVeryCleanCompilation is a number
isInteger $isVeryCleanCompilation

# Captures the return value of the previous function call command
isIntegerReturnValue=$?

# Notify an invalid USB port number passed as parameter.
if ! [ $isIntegerReturnValue -eq 1 ]
then
    usbPortNumberToInstallTemp=$usbPortNumberToInstall
    usbPortNumberToInstall=$isVeryCleanCompilation
    isVeryCleanCompilation=$usbPortNumberToInstallTemp
fi


# Notify an invalid file passed as parameter.
if ! [ -f $programFileToCompile ]
then
    printf "\nERROR! Could not to find your program '$programFileToCompile'!\n"
    printf "Please try again providing an correct program. Example:\n"
    printf "./compile.sh PROGRAM_NAME.cc\n"
    printHelp
    exit 1
fi


# To perform subscripts calls.
if [ $# -eq 0 ]
then
    printHelp
    exit 1
else
    if sh _copy.sh $programFileToCompile 0
    then
        if sh _make.sh $programFileToCompile $isVeryCleanCompilation a
        then
            if sh _process.sh $programFileToCompile 0
            then
                # Calculates whether the seconds program parameter contains the 'notsend' word
                contains $isVeryCleanCompilation "notsend"
                
                # Captures the return value of the previous function call command
                containsReturnValue=$?
                
                # To clear any last compilation data.
                if ! [ $containsReturnValue -eq 1 ]
                then
                    printf "\nExiting because the 'notsend' argument was supplied...\n"
                    printf "The COMPILE_AND_INSTALL.SH was executed successfully!\n"
                    showTheElapsedSeconds
                else
                    # To perform the last part, send the application to the board.
                    if sh _install.sh $programFileToCompile $usbPortNumberToInstall
                    then
                        printf "\nThe COMPILE_AND_INSTALL.SH was executed successfully!\n"
                        showTheElapsedSeconds
                    else
                        printf "\n_INSTALL.SH ERROR! Could not to install the program into the EPOSMotes2!\n"
                        printHelp
                        exit 1
                    fi
                fi
            else
                printf "\n_PROCESS.SH ERROR! Could not to process the objcopy!\n"
                printHelp
                exit 1
            fi
        else
            printf "\n_MAKE.SH ERROR! Could not to compile the program!\n"
            printHelp
            exit 1
        fi
    else
        printf "\n_COPY.SH ERROR! Could not to copy the initial files!\n"
        printHelp
        exit 1
    fi
fi


# Exits the program using a successful exit status code.
exit 0





