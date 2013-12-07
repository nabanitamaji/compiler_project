#set -x 
export llvm_lib=/home/nabanita/llvm/Debug+Asserts/lib
orig_bc=$1
file=${orig_bc%%.*}
tmp1=$file"_inlined.bc"
opt -load $llvm_lib/Inl.so -ml < $1 > $tmp1
lli $tmp1 $2
size1=`ls -sh $1|awk '{print $1}'|sed "s:K::g" `
size2=`ls -sh $tmp1|awk '{print $1}'|sed "s:K::g" `
diff=`expr $size2 - $size1`
echo "codesize growth= $diff"
