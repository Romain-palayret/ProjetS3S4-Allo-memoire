#!/bin/bash 


apt rdepends $1 > depForte.txt 2> /dev/null


sed -E 's/\(.*\)//' depForte.txt > transition  

sed -E '/.*(Dépend|Pre-Depends|Recommande).*/!d' transition > depForte.txt

sed -E 's/^.*://'  depForte.txt > transition 

sed -E 's/[[:space:]]//' transition > depForte.txt

sed -E 's/<.*>//' depForte.txt > transition 

sed '/^$/d'  transition  > depForte.txt 

cat transition > depForte.txt 


input="depForte.txt"
echo "" > suppression.txt
sed '/^$/d' suppression.txt >  transition
cat transition > suppression.txt


while IFS= read -r line
do

    dpkg -s  $line 2> /dev/null

    if [ $? -ne 0 ]
    then 
        echo $line >> suppression.txt
    fi

done < $input    

input="suppression.txt" 

while IFS= read -r line
do 
    sed -e "/$line/d" depForte.txt  > transition
    cat transition > depForte.txt
done < $input

echo " ET BEH ? $1 " 

cat depForte.txt



