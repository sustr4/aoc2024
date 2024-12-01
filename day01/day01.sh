#!/bin/bash

cat input.txt | awk '{print $1}' > c1.txt
cat input.txt | awk '{print $2}' > c2.txt

SUM=0

cat c1.txt | while read id; do
	OCC=`grep -c $id c2.txt`
	SCORE=`echo "$OCC * $id" | bc`
	SUM=$(( $SUM + $SCORE))
	echo $SCORE, $SUM
done
