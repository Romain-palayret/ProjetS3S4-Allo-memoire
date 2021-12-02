#ifndef RECUPERERPAQUET_H
#define RECUPERERPAQUET_H



Paquet * testRetourPaquet( char *nom); 


void testChaineChar(char truc[]); 


Paquet * creationPaquet( char  nom[] );

char *  copierValeurChaine( char chaineACopier[] , int tailleChaine); 

int dresserArborescencePaquet(Paquet *parent , char ***chaine , int *tailleListe); 

int  ajouterToutesLesRdependsDirectes(Paquet *parent);

char * recupererPriorite(char nom[]);

int cePaquetEstIlInstalle(char nom[]);



int dresserArborescenceFinale(Paquet *parent , char ***chaine , int *tailleListe); 

#endif 
