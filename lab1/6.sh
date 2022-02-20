#!/bin/bash
if [ ! "$1" -o ! "$2" -o ! "$3" ]; then
	echo "Not enough parameters: 'output file name' 'search dir name' 'extention'"
elif [ ! -d  "$2" ]; then
	echo "Invalid parameter: no such directory." >&2
else
	(find "$2" -type f -name "*.$3" | sort -i) > "$1"
fi
