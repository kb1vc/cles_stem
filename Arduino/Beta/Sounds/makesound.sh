#!/bin/bash

destdir=$1

echo "Copy sounds to " $destdir

cp digit*.wav ${destdir}
cp space000.wav ${destdir}
cp givecard.wav ${destdir}
cp pokebutn.wav ${destdir}
cp iamlost0.wav ${destdir}
cp klaxon*.wav ${destdir}
cp song000[0123].wav ${destdir}
cp calmode[es].wav ${destdir}
cp fanfare0.wav ${destdir}


