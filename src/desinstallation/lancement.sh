#!/bin/bash

var=$(pwd)

sed "s|.*#define.*CHEMINPROJET.*| #define CHEMINPROJET  \"$(pwd)\/\" |" programmeC/recupererPaquet.c > transition 
cat transition > programmeC/recupererPaquet.c 

rm transition


gcc $(pwd)/programmeC/*.c -o exe

programmeC/exe $1
