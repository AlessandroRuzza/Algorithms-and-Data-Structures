#! /usr/bin/sh
if [ $# -ge 1 ] && [ $1 -ge 0 ]
then
    input=$1
else
    echo "Number of input test-case: "
    read input
fi
cp ./Test/open_$input.txt .
cp ./Test/open_$input.output.txt .
