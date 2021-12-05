#ifndef RECUPERERPAQUET_H
#define RECUPERERPAQUET_H



Paquet * testRetourPaquet( char *nom , char cheminProjet[] ); 


void testChaineChar(char truc[]); 


Paquet * creationPaquet( char  nom[] , char cheminProjet[] );

char *  copierValeurChaine( char chaineACopier[] , int tailleChaine , char cheminProjet[]); 

int dresserArborescencePaquet(Paquet *parent , char ***chaine , int *tailleListe , char cheminProjet[]); 

int  ajouterToutesLesRdependsDirectes(Paquet *parent , char cheminProjet[]);

char * recupererPriorite(char nom[] , char cheminProjet[]);

int cePaquetEstIlInstalle(char nom[] , char cheminProjet[] );


int dresserArborescenceFinale(Paquet *parent , char ***chaine , int *tailleListe , char cheminProjet[]); 

#endif 
