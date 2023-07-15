#!/bin/sh
echo "Number of input test-case (0 to exit): "
read input

./main < Test/open_$input.txt > ./output_$input.txt