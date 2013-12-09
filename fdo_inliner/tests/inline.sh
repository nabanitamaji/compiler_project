#set -x 
ft=$1
shift
ct=$1
shift
orig_bc=$1
shift
file=${orig_bc%%.*}
tmp1=$file"_inlined.bc"

echo ""
echo "ft=$ft ct=$ct"
opt -O3 <$orig_bc>tmp.bc 2>/dev/null
opt -load $llvm_lib/Inl.so -ml -ft=$ft -ct=$ct<$orig_bc>$tmp1 2>/dev/null
opt -O3<$tmp1>tmp2.bc 2>/dev/null

for i in {1..5};do
	echo ""
	echo "original:"
	#time lli $1 $2>/dev/null
	time lli tmp.bc $@>/dev/null
	echo ""
	echo "inlined:"
	time lli tmp2.bc $@>/dev/null
	size1=`du -b $orig_bc|awk '{print $1}' `
	size2=`du -b $tmp1|awk '{print $1}'`
	diff=`expr $size2 - $size1`
	echo ""
	echo "codesize growth= $diff"
	echo ""
done
rm -f tmp.bc tmp2.bc
