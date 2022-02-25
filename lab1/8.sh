#!/bin/bash
if [ ! $1 -o ! $2 ]; then
	echo "Not enough arguments." >&2
else
for i in $1/*
do
	if [ -f $i ]; then
		for j in $2/*
		do
			if [ -f $j ]; then
				if  cmp -s $i $j ; then
					echo "$i == $j"
				fi
			fi
		done
	fi
done
echo "Number of viewed files is $(( `find $1 -maxdepth 1 -type f | wc -l`+`find $2 -maxdepth 1 -type f | wc -l` ))"
fi
