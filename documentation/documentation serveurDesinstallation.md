# Documentation du programme de desinstallation :

## Description générale
Le programme _serveurDesinstallation.c_ est un programme C s'occupant de l'appel du programme de désinstallation _dReccur_ toutes les X minutes.
Lancé à chaque démarrage de la machine avec les droits root, ce programme comporte une boucle infini.

## Outils utilisés :
Le programme _serveurDesinstallation.c_ a été codé sur l'editeur texte _vim_ et _gedit_ et testé sur l'environnement Debian

## Description de la conception et réalisation du script

### Algorithme utilisé :

```
- On lit la date courante
- dateCalcule =  date courante + le nombre de minute indiqué à la fin de la première ligne de serec.config

while(1==1) {
    - On lit la date courante
    if (dateCourante == dateCalcule) {
   
        - On execute le script dReccur chargé de la désinstallation
        - dateCalcule =  date courante + le nombre de minute indiqué à la fin de la première ligne de serec.config
   
    }
}
```


### fonction lectureConfig() :

La fonction lectureConfig() doit lire dans le fichier _serec.config_ et renvoyer le nombre de minute entre chaque désinstallation.
Ce nombre est le dernier mot de la premiere ligne du fichier _serec.config_.
Pour le récupérer, on lit la premiere ligne du fichier, puis cherche le premier digit de la ligne.
Une fois le digit trouvé, on suppose qu'il s'agit du dernier mot et que tous les caractères suivant sont des digits.
on convertit le reste de la ligne avec la fonction _atoi()_.

```
int lectureConfig () {

    char ligne[500];  
    int retour;
    FILE *fichier = fopen("/usr/bin/SeReC/serec.config" , "r");
    fgets(ligne , sizeof(ligne) , fichier);
   
    for(int i = 0 ; i < strlen(ligne) ; i++){
        if(ligne[i] > '0' && ligne[i] < '9'){
   return atoi(ligne+i);
}
    }

    return -1;

}
```
Si aucun digit n'est trouvé, on return -1. Aucune désinstallation n'aura lieu.


### fonction calculTemps() :

Ajouter des minutes à une date n'est pas facile. Par exemple, si on est le 28 fevrier à 23h 20 et qu'on souhaite ajouter 50 minutes, il faut modifier, le nombre de minute, l'heure, le jour et le mois.
Pour faciliter ces opérations, on utilise la librairie ```<time.h>```.
Cette librairie ajoute 2 structure permettant de manipuler les dates : time_t et tm.
_time\_t_ est une structure permettant de stocker une date et une heure.
_tm_ est une structrue permettant de stocker une date et une heure, mais sous un format normalisé.
Par exemple, une struct _time\_t_ peut contenir une heure ```12h 74min 89s``` alors qu'une struct _tm_ contiendra ```13h 15min 29s```.
La fonction _localtime()_ permet de passer d'une structure _time\_t_ à une structure _tm_


Le problème est que _<time.h>_ ne fournit pas une struct _tm_, mais seulement un pointeur vers cette dernière.
Cela à pour conséquence qu'il n'est possible de n'avoir qu'une seule struct tm à la fois.
Nous avons donc créer une structure _temp_ pour enregistrer les valeurs d'une structure _tm_ :
```
typedef struct temp{

  int min ;
  int h;
  int jour;
  int mois;
  int an;
}temp;
```

La fonction calculTemps() permet de mettre à jour une structure temps avec la date calcule à partir
de la date actuelle + le nombre de minutes situé dans le serec.config. 

On peut ensuite utiliser la structure ainsi initialisée pour initialiser la chaine Calculée par 
la suite. 

```
void calculTemps (temp *Temp){      
    struct tm *tempCalcule;
    time(&now);
    struct tm *local = localtime(&now); // local time ne fait QUE CONVERTIR
    calcule = now+60* lectureConfig();
    printf("\n  RETOUR DE LECTURE CONFIG %d \n" , lectureConfig());
    tempCalcule = localtime(&calcule);
    Temp->min = tempCalcule->tm_min;
    Temp->h = tempCalcule->tm_hour;
    Temp->jour = tempCalcule->tm_mday;
    Temp->mois = tempCalcule->tm_mon+1;
    Temp->an = tempCalcule->tm_year+1900;
}
```

## La fonction main 

La fonction main a pour objectif d'appliquer l'algorithme théorique présenté dans la première partie : 

````
    char dateCalcule[100]; 
    char dateNow[100];  
    struct temp temp_calcule; 
    calculTemps(&temp_calcule); 
    time(&now); 
    struct tm *local = localtime(&now); // local time ne fait QUE CONVERTIR 
    sprintf(dateCalcule , "%d:%02d:%02d:%02d:%02d" , temp_calcule.an , temp_calcule.mois , temp_calcule.jour , temp_calcule.h, 
		    temp_calcule.min); 
    sprintf(dateNow, "%d:%02d:%02d:%02d:%02d" , local->tm_year+1900 , local->tm_mon+1 , local->tm_mday , local->tm_hour ,
		    local->tm_min);
    printf(" la date maintenant : %s vs la date calculee : %s " , dateNow , dateCalcule); 
    printf(" \n la date maintenant : %02d vs la date calculee : %02d " , local->tm_min  , temp_calcule.min);
    
````
Cette partie consiste à initialiser la première itération avant de lancer la boucle. On met dans la chaine de 
caractère dateCalcule , la date qu'il fera dans x minutes de plus et dans la chaine date Now , la date courante à l'heure/minute
près. Une fois cela fait on peut rentrer dans la boucle de l'algorithme du début: 

````
    for(;;){
        time(&now);
        struct tm *local = localtime(&now); // local time ne fait QUE CONVERTIR 
        sprintf(dateNow, "%d:%02d:%02d:%02d:%02d" , local->tm_year+1900 , local->tm_mon+1 , local->tm_mday , local->tm_hour ,
                local->tm_min);
            if(strcmp(dateCalcule , dateNow) == 0){
           system("/usr/bin/SeReC/dReccur"); 
           calculTemps(&temp_calcule);
           sprintf(dateCalcule , "%d:%02d:%02d:%02d:%02d" , temp_calcule.an , temp_calcule.mois , temp_calcule.jour , temp_calcule.h, 
               temp_calcule.min); 
	}
````



