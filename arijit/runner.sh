#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please eneter the option"
	exit -1
fi
#llvm_lib=/home/arijit/Compiler/llvm-1.2.src/Debug+Asserts/lib
llvm_lib=/home/ari/Compiler/llvm-3.2.src/Debug+Asserts/lib

if [ $1 = "jpeg" ]
then
	#opt -load $llvm_lib/Inl.so -ml <jpeg-6a.bc> output.bc
	lli output.bc -dct int -progressive -opt ../testimg.ppm > img.jpeg
	#lli output.bc -quality 50 ../testimg.ppm > img.jpeg

elif [ $1 = "rawj" ]
then
	lli jpeg-6a.bc -dct int -progressive -opt ../testimg.ppm > img.jpeg

elif [ $1 = "mpeg" ]
then
	opt -load $llvm_lib/Inl.so -ml <mpeg2dec.bc> output.bc
	lli output.bc -b  ./input_base_4CIF_96bps.mpg -r -f -o0 ./tmp%d

elif [ $1 = "branch" ]
then
	clang -S -emit-llvm branchtest.cpp
	opt -load $llvm_lib/Inl.so -ml <branchtest.ll> output.bc
	llvm-dis output.bc 
	lli output.bc

elif [ $1 = "lua" ]
then
	opt -load $llvm_lib/Inl.so -ml <lua.bc> output.bc
	#lli output.bc arijit.lua

elif [ $1 = "clean" ]
then 
	echo "There are certain things to clean"
	rm *.ll
	rm output*
	rm *out*

fi
