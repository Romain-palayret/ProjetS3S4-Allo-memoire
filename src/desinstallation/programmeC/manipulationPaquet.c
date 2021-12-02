#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "manipulationPaquet.h"


/* Bloc des fonctions auxiliaires (elles n utilisent rien d autre qu elle même     */

void afficherRdependsDirectes(Paquet *parent){

    printf(" \n AFFICHAGE DES RDEPENDS DU PAQUET %s ---------------- \n " , parent->nomPaquetCourant); 
    for(int i = 0 ; i < parent->nombrePaquetDependant ; i++){ 
        printf(" \n element %d du paquet %s vaut %s " , i , parent->nomPaquetCourant , parent->reverseDep[i]->nomPaquetCourant); 
    }
    printf(" \n -------------------------- FIN AFFICHAGE DES RDEPENDS DU PAQUET %s ---------------------- \n\n" , parent->nomPaquetCourant); 
}



void afficherTableauChar(char **listeChar , int tailleChaine){
    
	
    int i = 0;

    printf("\n\n AFFICHAGE LISTE ---------- \n\n");  
    for(i = 0 ; i < tailleChaine ; i++){
        printf(" \n  element %d : vaut %s \n" , i , listeChar[i]);  
    }

    printf(" FIN DE L AFFICHAGE DE LA LISTE ------------- \n "); 
}



void initialiserPaquet(Paquet *parent , char *nomPaquet , char *Priorite){

    parent->nomPaquetCourant = nomPaquet; 
    parent->Priorite = Priorite; 

} 


/*
 *
 * AJoute sans controle prealable , un paquet a un autre (la relation est bien sûr rdepends) 
 * cette fonction est bas niveau , elle est utilise par la fonction officielle ajouterPaquetInverse
 * (qui elle fait les contrôles d entree) 
 */

void ajouterPaquetAPaquet(Paquet *parent , Paquet *paquetARajouter){

      Paquet  **listePaquetTransition = calloc(parent->nombrePaquetDependant , sizeof(void*));
      
      int i = 0;
      for(i = 0 ; i < parent->nombrePaquetDependant ; i++){
          listePaquetTransition[i] = parent->reverseDep[i];
      }
      parent->nombrePaquetDependant += 1;
      parent->reverseDep = calloc(parent->nombrePaquetDependant , sizeof(void*)); 

      for(i = 0 ; i < parent->nombrePaquetDependant-1 ; i++){
          parent->reverseDep[i] =  listePaquetTransition[i];
      }
      parent->reverseDep[i] = paquetARajouter;
}

/* 
 *
 *
 * Cette fonction prend en parametre un tableau de Chaine de caracte , et lui ajoute une 
 * chaine de caractere en modifiant la taille associe à cette chaine de caractere. 
 *
 *
 * */

void ajouterElementAListeChar(char ***listeChar , char *chaineACopier , int *tailleChaine){

    
    char **transition = calloc(*tailleChaine , sizeof(void*)); 
    int i = 0; 

    for(i = 0 ; i < *tailleChaine ; i++){
        transition[i] = (*listeChar)[i]; 
    }
    *tailleChaine += 1;
    *listeChar = calloc(*tailleChaine , sizeof(void*));  
    for(i = 0 ; i < *tailleChaine-1 ; i++){
	(*listeChar)[i] = transition[i];     
    }
    (*listeChar)[i] = chaineACopier;  
}



/*
 *
 * Cette fonction sert a verifier si un doublon est present dans un tableau de char 
 * elle sera utile , notamment pour le dressage ordonne de la liste de paquet a supprimer 
 *
 * Ici on ne fait pas de passage par adresse , on fait uniquement une copie des variables pour renvoyer 0 si 
 * la chaine n est pas presente , 1 si la chaine est presente 
 *
 */

int chaineDejaPresenteDansLeTableauDeChar( char **tableauChaine , char *chaineAVerifier  , int tailleListe){

   int i = 0; 
   int y = 0; 
   int valeurRetour = 0 ; 

   for(i = 0 ; i < tailleListe && valeurRetour != 1 ; i++){ 
       valeurRetour = (strcmp(tableauChaine[i] , chaineAVerifier) == 0) ? 1 : 0;    
   }
   return valeurRetour;  

}
      
/* ----------------------------  FONCTIONNALITES  PRINCIPALES -------------------------------------   */



/* 
 *
 * utilise ajouterPaquetAPaquet
 *
 * Cette fonction prend en argument un paquet parent , et lui ajoute le paquet enfant1 à son tableau d 
 * adresse pointant vers d'autres paquets. 
 *
 * */
void ajouterPaquetReverse ( Paquet *parent , Paquet *enfant){


    if(parent->reverseDep == NULL){
	parent->reverseDep = calloc(1 , sizeof(void*)); 
        parent->reverseDep[0] = enfant;
        parent->nombrePaquetDependant = 1; 
    }else{
        ajouterPaquetAPaquet(parent , enfant) ; 
        	
    }

}


/*
 * Cette fonction affiche de manière recursive , les paquets dependant du paquet parent;  
 *
 * */
void afficherDependanceInversePaquet(Paquet *parent){

    int i = 0 ; 
    printf(" \n --------------------- DEPENDANCES INVERSES DU PAQUET %s ------------------------ " , parent->nomPaquetCourant); 
    for(i = 0 ; parent->reverseDep != NULL &&  i < parent->nombrePaquetDependant ; i++){
        printf("\n le paquet parent %s a pour enfant %s    " , parent->nomPaquetCourant  , parent->reverseDep[i]->nomPaquetCourant); 
    }
    printf(" \n ------------------------- FIN DES DEPENDANCES INVERSES DU PAQUET %s ---------------------  \n\n" , parent->nomPaquetCourant); 
    
    if(parent->reverseDep != NULL){ 
    	for(i = 0 ; i < parent->nombrePaquetDependant ; i++){
        	afficherDependanceInversePaquet(parent->reverseDep[i]);  
    	}
    }
} 

/* 
 *
 * Pour le premier appel , il faut que la chaine que le premier element de la chaine soit nul   , 
 * 
 * Le but de cette fonction est de prendre un parent , l'adresse d un tableau de chaine de caractere ainsi que la taille de ce tableau en parametre , 
 * pour que ce tableau de chaine de caractere contienne la liste des paquets à supprimer; Si l on veut supprimer le paquet parent , il suffira
 * de supprimer les paquets en partant de la fin du tableau
 *
 * La liste ressemblera donc à ceci : { A , B , C , D , E } et pour supprimer A (le paquet parent sera toujours au début) on supprime E 
 * puis D , puis C , etc ... 
 *
 * */

void dresserListePaquetASupprimer(char ***chaine , int *tailleChaine , Paquet *parent){

    int i = 0; 
    if( (*chaine)[0] == NULL){
	*tailleChaine = 1; 
	*chaine = calloc(*tailleChaine , sizeof(void*));
	(*chaine)[0] = parent->nomPaquetCourant;	
    } 

    /* on ajoute les paquets (en allant pas sur les doublons )  */ 
    for(i = 0 ; i < parent->nombrePaquetDependant ; i++){
        if( parent->reverseDep != NULL && chaineDejaPresenteDansLeTableauDeChar(*chaine , parent->reverseDep[i]->nomPaquetCourant , *tailleChaine) != 1  ){
            ajouterElementAListeChar(chaine , parent->reverseDep[i]->nomPaquetCourant  , tailleChaine); 
	    dresserListePaquetASupprimer(chaine , tailleChaine , parent->reverseDep[i]); 
        }	
    }        
}

