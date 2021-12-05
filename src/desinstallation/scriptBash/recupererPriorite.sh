#!/bin/bash

echo "$2/scriptBash/priorite.txt" 


apt show $1 > $2/scriptBash/priorite.txt 2> /dev/null

sed -E  '/Priority:/!d' $2/scriptBash/priorite.txt > $2/scriptBash/transition

sed -E 's/[[:space:]]//'  $2/scriptBash/transition > $2/scriptBash/priorite.txt 

sed -E 's/.*://' $2/scriptBash/priorite.txt > $2/scriptBash/transition 

cat $2/scriptBash/transition > $2/scriptBash/priorite.txt 

rm $2/scriptBash/transition


cat $2/scriptBash/priorite.txt
