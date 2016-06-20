#!/usr/bin/env bash


#
# It updates the software version number.  indicated at "filePathToUpdate" and "versionFilePath".
# The versions at these two files must to be synchronized for a correct version update. 
#
#


# Import the helper functions.
. ./installer_files/__helper_functions.sh



githooksConfig=$(cat $PWD_COMPILE_EPOS_LAMP/installer_files/githooksConfig.txt)

# $versionFilePath example: githooks/GALILEO_SMA_VERSION.txt
versionFilePath=$PWD_COMPILE_EPOS_LAMP/$(echo $githooksConfig | cut -d',' -f 1)

# $filePathToUpdate example: scripting/galileo.sma
filePathToUpdate=$PWD_COMPILE_EPOS_LAMP/$(echo $githooksConfig | cut -d',' -f 2)


currentVersion=$(cat $versionFilePath)
originalVersion=$currentVersion



if [ -z "${currentVersion}" ]
then
    echo "VAR <$currentVersion> is bad set or set to the empty string"
    exit 1
fi


currentVersion=$(expr $currentVersion + 1)



# To prints a error message when it does not find the version number on the file.
#
# 'F' affects how PATTERN is interpreted (fixed string instead of a regex).
# 'q' shhhhh... minimal printing.
#
if ! grep -Fq "$originalVersion" "$filePathToUpdate"
then
    echo "Error! Could not find $originalVersion and update the file '$filePathToUpdate'."
    echo "The current version number on this file must be $originalVersion!"
    echo "Or fix the file '$versionFilePath' to the correct value."
    exit 1
fi



# Replace the file with the $versionFilePath with the $currentVersion.
echo $currentVersion > $versionFilePath
echo "Replacing the identification build number $originalVersion -> $currentVersion in '$filePathToUpdate'"
sed -i -- "s/$originalVersion/$currentVersion/g" $filePathToUpdate



# Exits the program using a successful exit status code.
exit 0


