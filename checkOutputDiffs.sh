#! /usr/bin/sh
echo "Number of input test-case (0 to exit): "
read input

diff ./output_$input.txt ./Test/open_$input.output.txt > diff_$input.txt