#!/bin/bash

#1440
#for i in $(seq -f "%04g" 1 1441)
for i in {1..1441}
do
	printf -v PADDED_NAME '%04d' $i
	TIME=$((1736658000 + (60*i)))
	echo "hey there! it is" `date -j -f"%s" $TIME +%-I:%M%p`|say -v "Zoe (Premium)" --output-file=aiff/$PADDED_NAME.aiff
done

for i in aiff/*;
do 	
	name=${i%.aiff}
	name=${name##*/}
	sox "$i" mp3/"${name}.mp3"
done
