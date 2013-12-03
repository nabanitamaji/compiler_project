#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please eneter the option"
	exit -1
fi
#llvm_lib=/home/arijit/Compiler/llvm-1.2.src/Debug+Asserts/lib
llvm_lib=/home/ari/Compiler/llvm-3.2.src/Debug+Asserts/lib
if [ $1 == "comp" ]
then 
	make
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc

elif [ $1 = "jpeg" ]
then
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	#lli output.bc -dct int -progressive -opt ./testimg.ppm > jpeg.out

elif [ $1 = "jpega" ]
then
	llvm-dis jpeg-6a.bc -o temp.ll
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	rm temp.ll
	lli output.bc -dct int -progressive -opt ./testimg.ppm > jpeg.out

elif [ $1 = "mpeg" ]
then
	opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	lli output.bc -b  ./input_base_4CIF_96bps.mpg -r -f -o0 ./tmp%d

elif [ $1 = "clean" ]
then 
	echo "There are certain things to clean"
	rm *.ll
	rm output*
	rm *out*

elif [ $1 = "arijit" ]
then
	clang  -emit-llvm -c arijit.cpp -o arijit.bc
	opt -load $llvm_lib/Inl.so -ml <arijit.bc> output.bc
fi
