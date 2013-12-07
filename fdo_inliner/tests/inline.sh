#set -x 
export llvm_lib=/home/nabanita/llvm/Debug+Asserts/lib
orig_bc=$1
file=${orig_bc%%.*}
ft=$3
ct=$4
tmp1=$file"_inlined.bc"

echo ""
echo "ft=$ft ct=$ct"
opt -O3 <$1>tmp.bc 2>/dev/null
opt -load $llvm_lib/Inl.so -ml -ft=$ft -ct=$ct<$1>$tmp1 2>/dev/null
opt -O3<$tmp1>tmp2.bc 2>/dev/null
for i in {1..5};do
	echo ""
	echo "original:"
	#time lli $1 $2>/dev/null
	time lli tmp.bc $2>/dev/null
	echo ""
	echo "inlined:"
	time lli tmp2.bc $2>/dev/null
	size1=`du -b $orig_bc|awk '{print $1}' `
	size2=`du -b $tmp1|awk '{print $1}'`
	diff=`expr $size2 - $size1`
	echo ""
	echo "codesize growth= $diff"
	echo ""
done
rm -f tmp.bc tmp2.bc
