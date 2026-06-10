#!/bin/bash

writefile=$1
writestr=$2

if [ $# -ne 2 ]
then
	echo -e "Error: Invalid number of arguments. \nWriter script needs 2 arguments in the following order: \n1)File full path. \n2)String to be written in the specified file."
	exit 1
fi

dirpath=$(dirname "$writefile")
mkdir -p "$dirpath"

if ! echo "${writestr}" > "$writefile"
then
	echo "Error: ${writefile} could not be created."
	exit 1
fi

exit 0

