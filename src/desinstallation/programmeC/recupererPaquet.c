#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manipulationPaquet.h"
#include "recupererPaquet.h"
#define FICHIERDEP "depForte.txt"
#define FICHIERALL "casse.txt" 
#define FICHIERPRIORITE "priorite.txt"
#define  TAILLE_CHAINE_CARAC 300




/*
 *
 * Cette fonction a pour but de verifier si l on peut renvoyer un 
 *
 */
Paquet * testRetourPaquet( char *nom , char cheminProjet[]){

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

void copierValeurLitterale(char *chaineDest , char *chaineSrc , int tailleMax){

    int i = 0; 
    for(i = 0 ; i < tailleMax ; i++){
	
        chaineDest[i] = chaineSrc[i];  
    }
}


void enleverRetourLigne(char *chaine , int taille){

    int i = 0; 
    for(i = 0 ; i < taille ; i++){
	 
        if(chaine[i] == '\n'){
	    chaine[i] = ' '; 
	}
    
    }    

}


char * copierValeurChaine( char chaineACopier[] , int tailleChaine , char cheminProjet[] ){

    int i = 0;
    char *chaine = calloc(tailleChaine , sizeof(char)) ; 
    for(i = 0 ; i < tailleChaine &&  chaineACopier[i] != '\0' ; i++){
        chaine[i] = chaineACopier[i];  
    }
    return chaine; 
}

Paquet * creationPaquet( char  nom[] , char cheminProjet[] ){
        
	

	Paquet *paquet = calloc(1 , sizeof(Paquet));
	paquet->Priorite = recupererPriorite(nom , cheminProjet);
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
	paquet->nomPaquetCourant = copierValeurChaine(nom , TAILLE_CHAINE_CARAC , cheminProjet);
	return paquet; 
}

char * recupererPriorite(char nom[] , char cheminProjet[]){

    /* on créer l emplacement du fichier  */
    char fichier[TAILLE_CHAINE_CARAC];
    copierValeurLitterale(fichier , cheminProjet , TAILLE_CHAINE_CARAC-5);  
    strcat(fichier , "/scriptBash/");
    strcat(fichier , FICHIERPRIORITE);  

    /* preparatiion de la commande   */
    char commande[TAILLE_CHAINE_CARAC];
    copierValeurLitterale(commande , cheminProjet , TAILLE_CHAINE_CARAC-5); 
    strcat(commande , "/scriptBash/recupererPriorite.sh ");  
    strcat(commande , nom); 
    strcat(commande , " ");
    strcat(commande , cheminProjet);

    enleverRetourLigne(commande , TAILLE_CHAINE_CARAC); 
    printf("\n\n\n COMMANDE RECUPERER PRIORITE : %s \n\n\n" , commande ); 
    
    
    char *retour = calloc(TAILLE_CHAINE_CARAC , sizeof(char) ) ; 
     
    if(system(commande) != 0  ){

        printf(" ERREUR "); 	
        return NULL; 
    }
    FILE *fichierLec = fopen(fichier , "r");


    if(  (fgets(retour , TAILLE_CHAINE_CARAC-5 , fichierLec)) == NULL ){
        return NULL; 
    } 

    return retour; 

}


/*
 * Creation arborescence niveau 1 
 */


int ajouterToutesLesRdependsDirectes(Paquet *parent , char cheminProjet[]){

   /* definition du chemin fichier  */

   char cheminFichier[TAILLE_CHAINE_CARAC]; 
   copierValeurLitterale(cheminFichier , cheminProjet , TAILLE_CHAINE_CARAC-5); 
   strcat(cheminFichier , "/scriptBash/"); 
   strcat(cheminFichier , FICHIERDEP);  



   /* definition des parametre de la commande  */ 
   char commande[TAILLE_CHAINE_CARAC];
   copierValeurLitterale(commande , cheminProjet  , TAILLE_CHAINE_CARAC-5); 
   strcat(commande , "/scriptBash/recupererDependanceForte.sh "); 
   strcat(commande , parent->nomPaquetCourant); 
   strcat(commande , " "); 
   strcat(commande , cheminProjet);   

   enleverRetourLigne(commande , TAILLE_CHAINE_CARAC-5); 
   printf("\n\n LA COMMANDE RECUPERER RDEPENDS  %s    \n\n " , commande);  

   /* on défini les differents chaines qui vont permettre 
    * d effectuer la lecture   */
   char chaine2[TAILLE_CHAINE_CARAC];
   char lecture[TAILLE_CHAINE_CARAC]; 
   Paquet *temporaire; 
   int i = 0; 


   printf(" \n --------------  LE NOM DU PAQUET ENTRE %s    ----------- \n  " , parent->nomPaquetCourant );  
   system(commande); // recuperation rdepends declenchement de la commande  
   FILE *fichier = fopen(cheminFichier , "r");
   if(fichier == NULL){
       return 0; 
   }  

   while( (fgets(lecture , TAILLE_CHAINE_CARAC , fichier)) != NULL  ){
	printf(" \n on ajoute le paquet %s depuis la fonction  " , lecture);
        temporaire = creationPaquet(lecture , cheminProjet);
	if(temporaire != NULL){	
            ajouterPaquetReverse(parent , creationPaquet(lecture , cheminProjet) );
	}else{
	    return -1; 
	}
   }

   return 0;    
}

int cePaquetEstIlInstalle(char nom[] , char cheminProjet[]){

    char commande[TAILLE_CHAINE_CARAC];
    copierValeurLitterale(commande ,   cheminProjet , TAILLE_CHAINE_CARAC-5);  
    strcat(commande , "/scriptBash/paquetInstalle.sh "); 
    strcat(commande , nom); 


    printf(" \n\n paquet Installe COMMANDE %s  \n\n" , commande); 

    return system(commande); 
} 


/* LA FONCTION FINALE   */

int  dresserArborescencePaquet(Paquet *parent , char ***chaineSuppression , int *tailleListe , char cheminProjet[]){


    	int i = 0; 
    	int paquetPresent = 0;
	int paquetInstalle = 0; 
    	int sauvegardeTailleListe = *tailleListe; 
        int retour = 0; 	
    	Paquet *temp = creationPaquet(parent->nomPaquetCourant , cheminProjet);

	if(temp == NULL){
	    return -1; 
	}

    	retour = ajouterToutesLesRdependsDirectes(temp , cheminProjet);

	if(retour == -1){
	    return -1; 	
	}

    	for(i = 0 ; i < temp->nombrePaquetDependant ; i++){
        	paquetPresent = chaineDejaPresenteDansLeTableauDeChar(*chaineSuppression , temp->reverseDep[i]->nomPaquetCourant , *tailleListe);
	        paquetInstalle = cePaquetEstIlInstalle(temp->reverseDep[i]->nomPaquetCourant , cheminProjet); 	
        	if(paquetPresent != 1 && paquetInstalle == 0){
	    		ajouterPaquetReverse(parent , temp->reverseDep[i]);
	    		ajouterElementAListeChar(chaineSuppression , temp->reverseDep[i]->nomPaquetCourant , tailleListe); 	    
		}
    	}

    	/* si on a reussi a rajouter des paquets   */
    	if(sauvegardeTailleListe < *tailleListe){
        	for(i = 0 ; i < parent->nombrePaquetDependant ; i++){
	    		retour = dresserArborescencePaquet(parent->reverseDep[i] , chaineSuppression , tailleListe , cheminProjet);  	   
		        if(retour == -1){
			    return -1; 
			}	
		}
    	}

	return 0; 
}


int dresserArborescenceFinale(Paquet *parent , char ***chaineSuppression , int *tailleListe , char cheminProjet[]){

	
	ajouterElementAListeChar(chaineSuppression , parent->nomPaquetCourant , tailleListe);
        int retour = dresserArborescencePaquet(parent , chaineSuppression , tailleListe , cheminProjet); 	
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



