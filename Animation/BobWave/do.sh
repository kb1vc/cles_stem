#!/bin/bash

for fi in *.svg
do
    svg2png $fi
done

ffmpeg -framerate 1/5 -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png -i bob0.png -i bob1.png -i bob2.png bob_wave.mpg

