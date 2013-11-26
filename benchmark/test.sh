#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Please enter the argument"
	exit
fi

command=$1

if [ $command == 'gen' ]
then
	llvm-dis g721encode.bc -o g721encode_before.ll
	opt -always-inline g721encode.bc -o g721Output.bc
	llvm-dis g721encode.bc -o g721encode_after.ll
	xxdiff g721encode_before.ll g721encode_after.ll
	
elif [ $command == 'jpeg' ]
then

	llvm-dis jpeg-6a.bc -o jpeg-6a_before.ll
	opt -always-inline jpeg-6a.bc -o jpeg-6aOutput.bc
	llvm-dis jpeg-6aOutput.bc -o jpeg-6a_after.ll
	xxdiff jpeg-6a_before.ll  jpeg-6a_after.ll 

elif [ $command == 'mpeg' ]
then

	llvm-dis mpeg2dec.bc -o mpeg2dec_before.ll
	opt -always-inline mpeg2dec.bc -o mpeg2decOutput.bc
	llvm-dis mpeg2decOutput.bc -o mpeg2dec_after.ll
	xxdiff mpeg2dec_before.ll g721encode_after.ll

elif [ $command == 'rawc' ]
then

	llvm-dis rawcaudio.bc -o rawcaudio_before.ll
	opt -always-inline rawcaudio.bc -o rawcaudioOutput.bc
	llvm-dis rawcaudioOutput.bc -o rawcaudio_after.ll
	xxdiff rawcaudio_before.ll rawcaudio_after.ll

elif [ $command == 'rawd' ]
then

	llvm-dis rawdaudio.bc -o rawdaudio_before.ll
	opt -always-inline rawdaudio.bc -o rawdaudioOutput.bc
	llvm-dis rawdaudioOutput.bc -o rawdaudio_after.ll
	xxdiff rawdaudio_before.ll rawdaudio_after.ll


elif [ $command == 'toast' ]
then

	llvm-dis toast.bc -o toast_before.ll
	opt -always-inline toast.bc -o toastOutput.bc
	llvm-dis toastOutput.bc -o toast_after.ll
	xxdiff toast_before.ll toast_after.ll
	
elif [ $command == 'clear' ] 
then
	rm *.ll
	rm *Output*
	rm *before*
	rm *after*

fi

