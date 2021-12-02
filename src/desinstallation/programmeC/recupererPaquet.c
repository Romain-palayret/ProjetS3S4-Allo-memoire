#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulationPaquet.h"
#include "recupererPaquet.h"
#define FICHIER "depForte.txt"
#define FICHIERALL "casse.txt" 
#define FICHIERPRIORITE "priorite.txt"
#define  TAILLE_CHAINE_CARAC 300


/*
 *
 * Cette fonction a pour but de verifier si l on peut renvoyer un 
 *
 */
Paquet * testRetourPaquet( char *nom){

    Paquet *parent = calloc(1 , sizeof(Paquet)); 
    Paquet *enfant1 = calloc(1 , sizeof(Paquet)); 
    Paquet *enfant2 = calloc(1 , sizeof(Paquet));


    parent->nomPaquetCourant = nom; 
    enfant1->nomPaquetCourant = "enfant1"; 
    enfant2->nomPaquetCourant = "enfant2"; 

    ajouterPaquetReverse(parent , enfant1); 
    ajouterPaquetReverse(parent , enfant2);

    return parent;  
}

/*
 *
 * creationPaquet 
 *
 * */


char * copierValeurChaine( char chaineACopier[] , int tailleChaine ){

    int i = 0;
    char *chaine = calloc(tailleChaine , sizeof(char)) ; 
    for(i = 0 ; i < tailleChaine &&  chaineACopier[i] != '\0' ; i++){
        chaine[i] = chaineACopier[i];  
    }
    return chaine; 
}

Paquet * creationPaquet( char  nom[] ){
        
	 
	Paquet *paquet = calloc(1 , sizeof(Paquet));
	paquet->Priorite = recupererPriorite(nom);
	if(paquet->Priorite == NULL){
	    printf("\n LE PAQUET %s ne peut pas etre recueilli  \n" , nom ); 
	    return NULL; 
	}else{
	    printf(" \n LE PAQUET %s est %s  \n  " , nom , paquet->Priorite); 
	
	}

	int required = strcmp(paquet->Priorite , "required\n"); 
	int important = strcmp(paquet->Priorite , "important\n"); 
	int standard = strcmp(paquet->Priorite , "standard\n");

	if(required == 0 || important == 0 || standard == 0){
             return NULL; 	
	}	
	paquet->nomPaquetCourant = copierValeurChaine(nom , TAILLE_CHAINE_CARAC);
	return paquet; 
}

char * recupererPriorite(char nom[]){


    char commande[TAILLE_CHAINE_CARAC] = "../scriptBash/recupererPriorite.sh "; 
    char *retour = calloc(TAILLE_CHAINE_CARAC , sizeof(char) ) ; 
    strcat(commande , nom); 
    if(system(commande) != 0  ){

        printf(" ERREUR "); 	
        return NULL; 
    }
    FILE *fichier = fopen(FICHIERPRIORITE , "r");
    if(  (fgets(retour , TAILLE_CHAINE_CARAC-5 , fichier)) == NULL ){
        return NULL; 
    }  
    return retour; 

}


/*
 * Creation arborescence niveau 1 
 */


int ajouterToutesLesRdependsDirectes(Paquet *parent){

   /* definition des parametre de la commande  */ 
   char chaine1[TAILLE_CHAINE_CARAC] = "../scriptBash/recupererDependanceForte.sh ";  
   char chaine2[TAILLE_CHAINE_CARAC];
   char lecture[TAILLE_CHAINE_CARAC]; 
   Paquet *temporaire; 
   int i = 0; 

   printf(" \n --------------  LE NOM DU PAQUET ENTRE %s    ----------- \n  " , parent->nomPaquetCourant );  
   strcat(chaine1 , parent->nomPaquetCourant);

   system(chaine1); 
   FILE *fichier = fopen(FICHIER , "r");
   if(fichier == NULL){
       return 0; 
   }  

   printf(" POURQUOI   ");
   while( (fgets(lecture , TAILLE_CHAINE_CARAC , fichier)) != NULL  ){
	printf(" \n on ajoute le paquet %s depuis la fonction  " , lecture);
        temporaire = creationPaquet(lecture);
	if(temporaire != NULL){	
            ajouterPaquetReverse(parent , creationPaquet(lecture) );
	}else{
	    return -1; 
	}
   }

   return 0;    
}

int cePaquetEstIlInstalle(char nom[]){

    char commande[TAILLE_CHAINE_CARAC] = "../scriptBash/paquetInstalle.sh "; 
    strcat(commande , nom); 
    return system(commande); 
} 


/* LA FONCTION FINALE   */

int  dresserArborescencePaquet(Paquet *parent , char ***chaineSuppression , int *tailleListe){


    	int i = 0; 
    	int paquetPresent = 0;
	int paquetInstalle = 0; 
    	int sauvegardeTailleListe = *tailleListe; 
        int retour = 0; 	
    	Paquet *temp = creationPaquet(parent->nomPaquetCourant);

	if(temp == NULL){
	    return -1; 
	}

    	retour = ajouterToutesLesRdependsDirectes(temp);

	if(retour == -1){
	    return -1; 	
	}

    	for(i = 0 ; i < temp->nombrePaquetDependant ; i++){
        	paquetPresent = chaineDejaPresenteDansLeTableauDeChar(*chaineSuppression , temp->reverseDep[i]->nomPaquetCourant , *tailleListe);
	        paquetInstalle = cePaquetEstIlInstalle(temp->reverseDep[i]->nomPaquetCourant); 	
        	if(paquetPresent != 1 && paquetInstalle == 0){
	    		ajouterPaquetReverse(parent , temp->reverseDep[i]);
	    		ajouterElementAListeChar(chaineSuppression , temp->reverseDep[i]->nomPaquetCourant , tailleListe); 	    
		}
    	}

    	/* si on a reussi a rajouter des paquets   */
    	if(sauvegardeTailleListe < *tailleListe){
        	for(i = 0 ; i < parent->nombrePaquetDependant ; i++){
	    		retour = dresserArborescencePaquet(parent->reverseDep[i] , chaineSuppression , tailleListe);  	   
		        if(retour == -1){
			    return -1; 
			}	
		}
    	}

	return 0; 
}


int dresserArborescenceFinale(Paquet *parent , char ***chaineSuppression , int *tailleListe){

	
	ajouterElementAListeChar(chaineSuppression , parent->nomPaquetCourant , tailleListe);
        int retour = dresserArborescencePaquet(parent , chaineSuppression , tailleListe); 	
	return retour; 
}


/*------------------------------------  TEST SUR LES CHAINES DE CARACTERE ------------------------------------  */

void testChaineChar (char truc[]){
	
    char chaineLocal[TAILLE_CHAINE_CARAC] ; 

   
    char chaine2[TAILLE_CHAINE_CARAC] = " ARARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; 
    char chaine3[TAILLE_CHAINE_CARAC] = " BORDEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEL"; 
    strcpy(chaineLocal , chaine2); 
    printf(" chaineLocal %s : et sa taille est de  %d  " , chaineLocal , strlen(chaineLocal)); 

    strcpy(chaineLocal , chaine3); 

    printf(" la chaine final : %s " , chaineLocal);  

}



