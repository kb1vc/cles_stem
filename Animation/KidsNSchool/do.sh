#!/bin/bash

for fi in kids*.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.jpg
done
