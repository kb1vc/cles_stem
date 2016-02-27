#!/bin/bash

for fi in AliceDesk.svg BobDesk.svg 
do
    bn=`basename $fi .svg`
    
    inkscape -f $fi  -e ${bn}.png
done
