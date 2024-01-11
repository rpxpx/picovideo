#!/bin/bash
# Return shortest argument
a=$1
for i in ${@:2}; do
    if [ ${#i} -lt ${#a} ]; then
	a=$i
    fi
done
echo $a
