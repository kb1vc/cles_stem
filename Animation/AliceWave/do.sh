#!/bin/bash

for fi in alice*.svg
do
    bn=`basename $fi .svg`
    
    inkscape -f $fi -e ${bn}.png
done

for (( i = 0; i < 5; i++))
do
    let a=$((i*4))
    an=`printf "%03d" $a`
    ln -s alice1.png alice_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`
    ln -s alice2.png alice_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`    
    ln -s alice1.png alice_w${an}.png
    let a=$((a+1))
    an=`printf "%03d" $a`    
    ln -s alice0.png alice_w${an}.png
done

for (( i = 0; i < 5; i++))
do
    let a=$((20+i))
    an=`printf "%03d" $a`
    ln -s alice_sm000.png alice_w${an}.png
done

for (( i = 0; i < 5; i++))
do
    let a=$((25+i))
    an=`printf "%03d" $a`
    ln -s alice_sm001.png alice_w${an}.png
done

for (( i = 0; i < 10; i++))
do
    let a=$((30+i))
    an=`printf "%03d" $a`
    ln -s alice_sm002.png alice_w${an}.png
done


rm -f alice_wave.mp4
ffmpeg -framerate 10  -i alice_w%03d.png -vcodec mpeg4 alice_wave.mp4

rm alice0.png 
rm alice1.png 
rm alice_w*.png