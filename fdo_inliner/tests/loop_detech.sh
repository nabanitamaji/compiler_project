#set -x 
orig_bc=$1
file=${orig_bc%%.*}
opt -load $llvm_lib/denyfunc.so -ld <$1> /dev/null
ls -ld denyfile.txt
