#! /usr/bin/sh
rm -r Main_TestResult
mkdir Main_TestResult
checkDiff="y"

if [ $# -ge 1 ]
then
    input=$1
else
    echo -n "Start from: "
    read input
    if [ $input -le 0 ]
    then 
        input=1
    fi
fi
if [ $# -ge 2 ]
then
    endAt=$2
else
    echo -n "End at: "
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
if [ $endAt -ge 99 ]
then
    input="extra_gen"
    echo "Testing #: $input" 
    ./main < Test/open_$input.txt > ./Main_TestResult/open_$input.output.txt
    if [ $checkDiff = "y" ]
    then 
        diff ./Main_TestResult/open_$input.output.txt ./Test/open_$input.output.txt > ./Main_TestResult/open_$input.diff.txt 
    fi
fi