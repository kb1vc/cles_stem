#!/bin/bash

for fi in bob*.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.png
done

for (( i = 0; i < 5; i++))
do
    let a=$((i*4))
    an=`printf "%03d" $a`
    ln -s bob1.png bob_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`
    ln -s bob2.png bob_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`    
    ln -s bob1.png bob_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`    
    ln -s bob0.png bob_w${an}.png
done

rm -f test.avi 
ffmpeg -framerate 10  -i bob_w%03d.png -vcodec mpeg4 bob_wave.avi

rm bob*.png 

