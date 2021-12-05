# Documentation de serec.config :

## Description générale
Le fichier serec.config est un fichier texte qui doit contenir toutes les informations nescessaire pour que notre logiciel s'adapte à la machine de l'utilisateur.
Chaque ligne correspond à une information. Les différents programmes du logiciel récupèrent le dernier mot de chaque ligne.

Ce fichier contient :
- le temps en seconde au bout duquel un paquet doit être désinstallé.
- le chemin absolu où seront stockées les listes Rouge, Orange et Verte
- le chemin absolu ou sera stocké le log contenant l'historique des différentes installations et désinstallations causé par notre logiciel.
- le chemin absolu du dossier de desinstallation

Cette liste sera amenée à évoluer dans le futur.

![alt text](./Image/serecConfig.png)
