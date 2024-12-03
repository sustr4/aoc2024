#!/bin/bash

grep -o 'mul([0-9][0-9]*,[0-9][0-9]*)' input.txt | awk -F'[(,)]' 'BEGIN {sum=0} {sum += $2 * $3} END {print sum}'

