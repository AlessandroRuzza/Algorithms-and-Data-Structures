#! /usr/bin/sh

if [ $# -ge 1 ]
then
    checkDiff=$1
else
    echo "Check for diff? (y/n): "
    read checkDiff
fi
if [ $# -ge 2 ]
then
    input=$2
else
    echo "Start from: "
    read input
    if [ $input -le 0 ]
    then 
        input=1
    fi
fi
if [ $# -ge 3 ]
then
    endAt=$3
else
    echo "End at: "
    read endAt
    if [ $endAt -ge 100 ]
    then 
        endAt=99
    fi
fi

while [ $input -le $endAt ]
do
    echo "Testing #: $input" 
    ./main < Test/open_$input.txt > ./Main_TestResult/open_$input.output.txt
    if [ $checkDiff = "y" ]
    then 
        diff ./Main_TestResult/open_$input.output.txt ./Test/open_$input.output.txt > ./Main_TestResult/open_$input.diff.txt 
    fi
    input=$(( $input + 1 ))
done
