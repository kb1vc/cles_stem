#!/bin/bash

for fi in BAw_0[01][0-9]*.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.png
done

for i in `seq 18 30`
do
    a=`printf "%03d" $i`
    ln -s BAw_017.png BAw_$a.png
done


rm -f BA_walk.avi 
ffmpeg -v 48 -framerate 3  -i BAw_%03d.png -c:v huffyuv BA_walk.avi

rm *.png 

