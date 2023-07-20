#! /usr/bin/sh
if [ $# -ge 1 ] && [ $1 -ge 0 ]
then
    input=$1
else
    echo -n "Number of input test-case: "
    read input
fi
./main < Test/open_$input.txt > ./output_$input.txt

if [ $# -ge 2 ]
then
    checkDiff=$2
else
    echo -n "Check for diff? (y/n): "
    read checkDiff
fi

if [ $checkDiff = "y" ]
then 
    diff ./output_$input.txt ./Test/open_$input.output.txt > open_$input.diff.txt 
    rm ./output_$input.txt
fi
