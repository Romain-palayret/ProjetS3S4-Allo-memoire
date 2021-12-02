#!/bin/bash

apt rdepends $1 | sed -E "s/\(.*\)//" > transition.txt 2> /dev/null 


sed -E "s/^.*://" transition.txt > rdep.txt


sed "/^$1/d" rdep.txt > transition.txt 


sed "/^$/d" transition.txt > rdep.txt 


sed "s/[[:space:]]//" rdep.txt > transition.txt 

cat transition.txt > rdep.txt

rm transition.txt 

