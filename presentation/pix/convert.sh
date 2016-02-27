#!/bin/bash

for fi in AliceDesk.svg BobDesk.svg HarrietTester.svg Teacher.svg Artist.svg BizGuy.svg BugReport.svg Lawyer.svg
do
    bn=`basename $fi .svg`
    
    convert $fi ${bn}.png
done
