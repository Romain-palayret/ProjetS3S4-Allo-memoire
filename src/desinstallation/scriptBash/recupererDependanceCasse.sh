#!/bin/bash 



apt depends $1 > casse.txt 2> /dev/null


echo " TOUT VA BIEN " 

sed -E  's/\(.*\)//' casse.txt > transition



sed -E '/Casse:/!d' transition > casse.txt




sed -E 's/.*://' casse.txt > transition 




sed -E 's/[[:space:]]//' transition > casse.txt 

sed -E '/<.*>/d' casse.txt > transition  


sed -E '/^$/d' transition > casse.txt






