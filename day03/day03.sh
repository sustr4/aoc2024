#!/bin/bash

grep -o -e 'mul([0-9][0-9]*,[0-9][0-9]*)' -e 'do()' -e "don't()" input.txt | while read l; do
	if [ "$l" == "do()" ]; then
		DO=""
	elif [ "$l" == "don't()" ]; then
		DO="not"
	else
		if [ "$DO" == "" ]; then
			echo $l
		fi
	fi
done | awk -F'[(,)]' 'BEGIN {sum=0} {sum += $2 * $3} END {print sum}'

