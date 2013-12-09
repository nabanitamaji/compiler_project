set -x
args=$#
ft=$1
ct=$2
echo $args
shift
shift
./gen_profile.sh $@
./loop_detech.sh $1
./inline.sh $ft $ct $@
