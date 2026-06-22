#!/bin/sh

filesdir=$1
searchstr=$2

if [ $# -ne 2 ]
then
	echo -e "Error: Invalid number of arguments. \nFinder script needs 2 arguments in the following order: \n1)File directory path. \n2)String to be searched in the specified directory path."
	exit 1
fi

if [ ! -d $filesdir ]
then
	echo "Error: File directory path does not represent a directory on the filesystem."
	exit 1
fi

set -e

filescount=$(find $filesdir -mindepth 1 -maxdepth 1 | wc -l)
linescount=0

for item in $filesdir/*
do
	if [ ! -d "$item" ]
	then
		let linescount=linescount+$(grep "${searchstr}" "$item" -c)
	fi
done

echo "The number of files are ${filescount} and the number of matching lines are ${linescount}."
exit 0

