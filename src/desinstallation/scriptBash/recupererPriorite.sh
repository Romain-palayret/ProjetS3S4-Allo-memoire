#!/bin/bash



apt show $1 > priorite.txt 2> /dev/null

sed -E  '/Priority:/!d' priorite.txt > transition

sed -E 's/[[:space:]]//'  transition > priorite.txt 

sed -E 's/.*://' priorite.txt > transition 

cat transition > priorite.txt 

rm transition
