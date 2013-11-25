#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please eneter the option"
	exit -1
fi
llvm_lib=/home/arijit/llvm/latest/llvm/Debug+Asserts/lib

if [ $1 == "comp" ]
then 
	make
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc

elif [ $1 = "jpeg" ]
then
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	lli output.bc -dct int -progressive -opt ./testimg.ppm > jpeg.out

elif [ $1 = "mpeg" ]
then
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	lli output.bc -b  ./input_base_4CIF_96bps.mpg -r -f -o0 ./tmp%d

elif [ $1 = "clean" ]
then 
	echo "There is nothing to clean"
fi
