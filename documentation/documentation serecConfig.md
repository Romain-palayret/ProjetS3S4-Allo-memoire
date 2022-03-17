# Documentation de serec.config :

## Description générale
Le fichier serec.config est un fichier texte qui doit contenir toutes les informations nescessaire pour que notre logiciel s'adapte à la machine de l'utilisateur.
Chaque ligne correspond à une information. Les différents programmes du logiciel récupèrent le dernier mot de chaque ligne.

Ce fichier contient :
- le temps en seconde au bout duquel un paquet doit être désinstallé.
- le chemin absolu où seront stockées les listes Rouge, Orange et Verte
- le chemin absolu ou sera stocké le log contenant l'historique des différentes installations et désinstallations causé par notre logiciel.
- le chemin absolu de l'application
- la liste des profils pris en compte ou non dans la création de la listeVerte.

## Description de la liste des profils
Le script _creeListeVerte_ a besoin des 6 à 11 pour fonctionner.
Si le chiffre à la fin d'une de ces lignes est égale à 0, alors on peut supprimer les paquets de la liste Orange Correspondante.
Si au contraire le chiffre à la fin d'une de ces lignes est égale à 1, alors ill faut prendre en compte la liste Orange correspondante lors de la création de la liste Verte.

Ces valeurs peuvent être modifiée à la main, directement dans le _serec.config_, ou bien en lançant le script _ajoutUser_.
Le script _ajoutUser_ à besoin d'une ligne vide à la fin du fichier pour fonctionner, c'est pourquoi il ne faut pas supprimer la dernière ligne.

```
temps de desinstallation : 30
emplacement des listes : /etc/SeReC/SeReC_listes
emplacement du Log : /etc/SeReC/SeReC_log
emplacement de Serec : /usr/bin/SeReC

Liste des profils :
Administrateur : 0
Secretaire : 0
Gamer : 0
Mathematicien : 0
Ingenieur Audio-visuel : 0


--Ne pas supprimer cette ligne--
```
