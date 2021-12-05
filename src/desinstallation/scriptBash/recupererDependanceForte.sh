#!/bin/bash 


apt rdepends $1 > $2/scriptBash/depForte.txt 2> /dev/null


sed -E 's/\(.*\)//' $2/scriptBash/depForte.txt > $2/scriptBash/transition  

sed -E '/.*(Dépend|Pre-Depends|Recommande).*/!d' $2/scriptBash/transition > $2/scriptBash/depForte.txt

sed -E 's/^.*://'  $2/scriptBash/depForte.txt > $2/scriptBash/transition 

sed -E 's/[[:space:]]//' $2/scriptBash/transition > $2/scriptBash/depForte.txt

sed -E 's/<.*>//' $2/scriptBash/depForte.txt > $2/scriptBash/transition 

sed '/^$/d'  $2/scriptBash/transition  > $2/scriptBash/depForte.txt 

cat $2/scriptBash/transition > $2/scriptBash/depForte.txt 


input=$2/scriptBash/depForte.txt
echo "" > $2/scriptBash/suppression.txt
sed '/^$/d' $2/scriptBash/suppression.txt >  $2/scriptBash/transition
cat $2/scriptBash/transition > $2/scriptBash/suppression.txt


while IFS= read -r line
do

    dpkg -s  $line 2> /dev/null
    if [ $? -ne 0 ]
    then 
        echo $line >> $2/scriptBash/suppression.txt
    fi

done < $input    

input=$2/scriptBash/suppression.txt 

while IFS= read -r line
do 
    sed -e "/$line/d" $2/scriptBash/depForte.txt  >  $2/scriptBash/transition
    cat $2/scriptBash/transition > $2/scriptBash/depForte.txt 
done < $input

echo " ET BEH ? $1 \n\n\n " 

cat $2/scriptBash/depForte.txt



