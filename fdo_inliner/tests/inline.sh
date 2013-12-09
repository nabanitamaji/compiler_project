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
	#time lli $1 $2>/dev/null
#	time lli tmp.bc $@>/dev/null
	t1=`echo $((time lli tmp.bc $@ >/dev/null) 2>&1)|awk '{print $2}'|sed 's:m::g'|sed 's:s::g'`
	echo "original: $t1 s"
	echo ""
	#time lli tmp2.bc $@>/dev/null
	t2=`echo $((time lli tmp2.bc $@ >/dev/null) 2>&1)|awk '{print $2}'|sed 's:m::g'|sed 's:s::g'`
	echo "inlined: $t2 s"
	size1=`du -b $orig_bc|awk '{print $1}' `
	size2=`du -b $tmp1|awk '{print $1}'`
	diff=`expr $size2 - $size1`
	diff2=`echo "scale=3; ($t1 - $t2)"|bc -l`
	incr=`echo "scale=3;($diff2 * 100) / $t1"|bc -l `
	echo ""
	echo "codesize growth= $diff"
	echo ""
	echo "time improvement= $diff2"
	echo "percentage = $incr"
	echo ""
done
rm -f tmp.bc tmp2.bc
