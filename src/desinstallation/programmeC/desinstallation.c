#include <stdio.h> 
#include <stdlib.h> 





void desinstallerListe(char **liste , int tailleListe){

    char chaine[100] = "../scriptBash/desinstaller.sh ";
    char chaineLecture[100];  
    for(int i = 0 ; i >= 0 ; i--){
        chaineLecture = ""; 
	chaineLecture = chaine; 
	strcat(chaineLecture , chaine); 
	system(chaineLecture); 
    }
}
