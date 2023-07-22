#! /usr/bin/sh
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
echo "Printing diffs from $input to $endAt"
while [ $input -le $endAt ]
do
    echo $input
    cat ./Main_TestResult/open_$input.diff.txt
    input=$(( $input + 1 ))
done
input="extra_gen"
echo $input
cat ./Main_TestResult/open_$input.diff.txt