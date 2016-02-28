#!/bin/bash -v



for fi in WB_rocket*.svg
do
    bn=`basename $fi .svg`
    echo "converting $fi"
    inkscape -f $fi  -e ${bn}.png
    sleep 1
done

let l=0
for (( i = 0; i < 20; i++))
do
    an=`printf "%03d" $l`
    ln -s WB_rocket${i}.png WB_rocket_${an}.png
    l=$((l+1))
done

for (( k = 0; k < 5; k++))
do
    for (( i = 17; i <= 19; i++ ))
    do
	an=`printf "%03d" $l`
	ln -s WB_rocket${i}.png WB_rocket_${an}.png
	l=$((l+1))
    done
done

    
rm -f WB_rocket.mp4
#ffmpeg -framerate 10  -i WB_rocket_%03d.png -c:v huffyuv WB_rocket.avi
ffmpeg -framerate 10  -i WB_rocket_%03d.png -vcodec mpeg4 WB_rocket.mp4




# this is the opening frame
inkscape -f  WB_clear.svg -e WB_clear.png

inkscape -f  WB_rocket19.svg -e WB_rocket_final.png

