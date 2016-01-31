#!/bin/bash

for fi in phb*.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.png
done
let l=0;
for (( i = 0; i < 5; i++))
do
    for (( k = 0 ; k < 9; k++))
    do
	a=$((i*9+k))
	an=`printf "%03d" $a`
	ln -s phb${k}.png phb_t${an}.png
	let l=$((l+1))
    done
done


rm -f phb_talk.avi 
ffmpeg -framerate 15  -i phb_t%03d.png -vcodec mpeg4 phb_talk.avi

rm phb*.png 

