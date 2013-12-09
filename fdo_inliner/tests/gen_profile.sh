#set -x 
export llvm_lib=/home/nabanita/llvm/Debug+Asserts/lib
orig_bc=$1
file=${orig_bc%%.*}
tmp1=$file"_combined.bc"
tmp2=$file"_final.bc"
clang -emit-llvm -c ../profile_generator/profilellvm.c -o ./profilellvm.bc
llvm-link profilellvm.bc $orig_bc -o $tmp1
opt -load $llvm_lib/fdo.so -gen < $tmp1 > $tmp2
lli $tmp2 $2 
awk '{print $3}' tracedump.log|sort -n|tail -1
