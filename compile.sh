#!/bin/bash
if [ $# -ne 1 ]
then
	echo "Please eneter the option"
	exit -1
fi

if [ $1 == "comp" ]
then 
	make
	opt -load $llvm_lib/fdo.so -dcg <jpeg-6a.bc> output.bc

elif [ $1 = "jpeg" ]
then
	opt -load $llvm_lib/fdo.so -gen <jpeg-6a.bc> output.bc
	clang -c -emit-llvm profile.c -o writer.bc
	llvm-link output.bc writer.bc -o one.bc
	#llvm-link output.bc writer.bc -o one.bc
	lli one.bc -dct int -progressive -opt ./testimg.ppm > jpeg.out

elif [ $1 == "clear" ]
then 
	rm output*
	rm one.bc
	rm wc

elif [ $1 == 'profile' ]
then
	clang -emit-llvm -c profilellvm.c -o profilellvm.bc
	llvm-link jpeg-6a.bc profilellvm.bc -o combine.bc

	opt -load $llvm_lib/fdo.so -gen <combine.bc> final.bc
	#lli final.bc -dct int -progressive -opt ./testimg.ppm > img.jpeg
	lli final.bc -quality 50 ./testimg.ppm > img.jpeg

elif [ $1 = "arijit" ]
then
	clang  -emit-llvm -c arijit.cpp -o arijit.bc
	opt -load $llvm_lib/fdo.so -dcg <arijit.bc> output.bc
fi
