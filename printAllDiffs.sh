#! /usr/bin/sh
input=1
echo "Printing diffs from 1 to 99"
while [ $input -le 99 ]
do
    echo $input
    cat ./Main_TestResult/open_$input.diff.txt
    input=$(( $input + 1 ))
done