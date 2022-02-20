#!/bin/bash
if [ $# -ne 2 ]; then
	echo "Not enough arguments." >&2
else
for i in $1/*
do
	for j in $2/*
	do
		if  cmp -s $i $j ; then
			echo "$i == $j"
		fi

	done
done
echo "Number of viewed files is $(( `ls $1 | wc -l` + `ls $2 | wc -l` ))"
fi
