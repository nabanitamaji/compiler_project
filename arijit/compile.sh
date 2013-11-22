#!/bin/bash

if [ $# == 1 ]
then
	command=$1
	if [ $command == 'dis' ]
	then
          	llvm-dis output.bc
	fi
else
	clang  -emit-llvm -c branchtest.c  -o branchtest.bc
	opt -load ../../../../Debug+Asserts/lib/Inl.so  -ml <branchtest.bc>output.bc
fi

