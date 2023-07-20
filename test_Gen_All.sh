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
    ./main < Gen_Test/open_$input.txt > ./Gen_Test_TestResult/open_$input.output.txt
    input=$(( $input + 1 ))
done
