#!/bin/bash



for fi in WB_rocket*.svg
do
    bn=`basename $fi .svg`
    
    inkscape -f $fi  -e ${bn}.png
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

    
rm -f WB_rocket.avi 
#ffmpeg -framerate 10  -i WB_rocket_%03d.png -c:v huffyuv WB_rocket.avi
ffmpeg -framerate 10  -i WB_rocket_%03d.png -vcodec mpeg4 WB_rocket.avi

rm WB_rocket*.png 



for fi in WB_robot[0-9].svg
do
    bn=`basename $fi .svg`
    convert $fi ${bn}_still.png
done

for fi in WB_robot_run_*.svg
do
    bn=`basename $fi .svg`
    convert $fi ${bn}.png
done

let l=0
for (( i = 0; i < 5; i++))
do
    an=`printf "%03d" $l`
    ln -s WB_robot${i}_still.png WB_robot_${an}.png
    l=$((l+1))
done

for (( i = 0; i < 20; i++))
do
    an=`printf "%03d" $l`
    ln -s WB_robot_run_${i}.png WB_robot_${an}.png
    l=$((l+1))
done

rm WB_robot.avi
ffmpeg -framerate 2  -i WB_robot_%03d.png -vcodec mpeg4 WB_robot.mp4

rm WB_robot*.png

# this is the opening frame
inkscape -f WB_robot0.svg -e WB_robot_run_first.png
inkscape -f WB_robot_run_19.svg -e WB_robot_run_last.png



# this is the opening frame
inkscape -f  WB_clear.svg -e WB_clear.png
inkscape -f  WB_rocket19.svg -e WB_rocket_final.png
