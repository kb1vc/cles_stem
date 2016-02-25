#!/bin/bash

for fi in *.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.png
done

