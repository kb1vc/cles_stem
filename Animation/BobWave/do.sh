#!/bin/bash

for fi in bob*.svg
do
    bn=`basename $fi .svg`
    
    inkscape -f $fi -e ${bn}.png
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

rm -f bob_wave.mov
#ffmpeg -framerate 10  -i bob_w%03d.png -vcodec mpeg4 bob_wave.avi
#ffmpeg -framerate 10  -i bob_w%03d.png -c:v libx264 -crf 20 -pix_fmt yuv420p bob_wave.mov
#ffmpeg -framerate 10  -i bob_w%03d.png -vcodec mpeg4 bob_wave.mp4
#avconv -i "bob_w%03d.png" -r 10 -c:v libx264 -crf 20 -pix_fmt yuv420p bob_wave.mov
#avconv -i "bob_w%03d.png" -r 10 -c:v libx264  -pix_fmt yuv420p bob_wave.mov
avconv -i "bob_w%03d.png" -r 10 -c:v mpeg1video  -pix_fmt yuv420p bob_wave.mov
rm bob0.png 
rm bob1.png 
rm bob_w*.png
