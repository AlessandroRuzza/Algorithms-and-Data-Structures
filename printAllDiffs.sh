#! /usr/bin/sh
if [ $# -ge 1 ] && [ $1 -ge 1 ]
then
    cat ./open_$1.diff.txt
else
    input=1
    echo "Printing diffs from 1 to 99"
    while [ $input -le 99 ]
    do
        echo $input
        cat ./Main_TestResult/open_$input.diff.txt
        input=$(( $input + 1 ))
    done
fi