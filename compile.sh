#!/bin/bash
set -x
if [ $# -ne 1 ]
then
	echo "Please eneter the option"
	exit -1
fi

if [ $1 == "clear" ]
then 
	rm output*
	rm one.bc
	rm wc

elif [ $1 == 'profile' ]
then
	clang -emit-llvm -c profilellvm.c -o profilellvm.bc
	llvm-link jpeg-6a.bc profilellvm.bc -o combine.bc

	opt -load $llvm_lib/fdo.so -gen <combine.bc> final.bc
	lli final.bc -dct int -progressive -opt ./testimg.ppm > img.jpeg
	#lli final.bc -quality 50 ./testimg.ppm > img.jpeg

elif [ $1 == 'lua' ]
then
	clang -emit-llvm -c profilellvm.c -o profilellvm.bc
	llvm-link arijit/lua.bc profilellvm.bc -o combine.bc
	opt -load $llvm_lib/fdo.so -gen <combine.bc> final.bc
	lli final.bc arijit/pi.lua
	#lli final.bc -quality 50 ./testimg.ppm > img.jpeg

elif [ $1 = "arijit" ]
then
	clang -emit-llvm -c profilellvm.c -o profilellvm.bc
	llvm-link arijit/test.bc profilellvm.bc -o combine.bc
	opt -load $llvm_lib/fdo.so -gen <combine.bc> final.bc
	lli final.bc
fi
