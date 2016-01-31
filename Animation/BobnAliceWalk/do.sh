#!/bin/bash

for fi in BAw_0[01][0-9]*.svg
do
    bn=`basename $fi .svg`
    
    inkscape -f $fi  -e ${bn}.png
done

for i in `seq 18 30`
do
    a=`printf "%03d" $i`
    ln -s BAw_017.png BAw_$a.png
done


rm -f BA_walk.mp4
ffmpeg -v 48 -framerate 3  -i BAw_%03d.png -vcodec mpeg4 BA_walk.mp4

rm *.png 

