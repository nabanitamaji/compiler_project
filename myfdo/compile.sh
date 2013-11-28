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
	opt -load $llvm_lib/fdo.so -dcg <jpeg-6a.bc> output.bc

elif [ $1 = "jpeg" ]
then
	opt -load $llvm_lib/fdo.so -dcg <jpeg-6a.bc> output.bc
	clang -c -emit-llvm write.c -o writer.bc
	llvm-link output.bc writer.bc -o one.bc
	lli one.bc -dct int -progressive -opt ./testimg.ppm > jpeg.out

elif [ $1 == "clear" ]
then 
	rm output*
	rm one.bc
	rm wc

elif [ $1 = "arijit" ]
then
	clang  -emit-llvm -c arijit.cpp -o arijit.bc
	opt -load $llvm_lib/fdo.so -dcg <arijit.bc> output.bc
fi
