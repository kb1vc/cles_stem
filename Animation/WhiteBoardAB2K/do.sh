#!/bin/bash -v



for fi in WB_robot[0-9].svg
do
    bn=`basename $fi .svg`
    inkscape -f $fi -e ${bn}_still.png
    sleep 1
done

for fi in WB_robot_run_*.svg
do
    bn=`basename $fi .svg`
    inkscape -f $fi -e ${bn}.png
    sleep 1
done


# rm WB_robot*.png

# this is the opening frame
inkscape -f WB_robot0.svg -e WB_robot_run_first.png

inkscape -f WB_robot_run_19.svg -e WB_robot_run_last.png


