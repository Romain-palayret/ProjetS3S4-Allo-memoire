#ifndef MANIPULATIONPAQUET_H
#define MANIPULATIONPAQUET_H

typedef struct Paquet{


    struct Paquet **reverseDep; 
    char *nomPaquetCourant; 
    char *Priorite; 
    int nombrePaquetDependant; 

}Paquet; 

/* fonctionnalités officielles   */


void ajouterPaquetReverse (Paquet *parent , Paquet *enfant); 

void afficherDependanceInversePaquet(Paquet *parent); 

void dresserListePaquetASupprimer(char ***chaine , int *tailleChaine , Paquet *parent);

void initialiserPaquet(Paquet *parent , char *nomPaquet , char *Priorite); 

/* fonctionnalités secondaire pouvant constituer les fonctionnalités officielles   */

int chaineDejaPresenteDansLeTableauDeChar( char **tableauChaine , char *chaineAVerifier  , int tailleListe); 

void ajouterElementAListeChar(char ***listeChar , char *chaineACopier , int *tailleChaine); 

void afficherTableauChar(char **listeChar , int tailleChaine); 




#endif 
