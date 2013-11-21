#!/bin/bash
 clang  -emit-llvm -c branchtest.c  -o branchtest.bc
 opt -load ../../../../Debug+Asserts/lib/Inl.so  -ml <branchtest.bc>output.bc

