# Documentation de l'accès aux listes

## Description générale
Le but de ce script est de pouvoir accéder aus listes : Verte, Orange et Rouge de n'importe où dans le répertoire. Pour cela il faut ajouter dans la variables d'environement PATH le chemin d'accès à ces scripts. Cette action est faite dans le script ***lancer*** avec sa documentation.

## Description de la conception et réalisation du script
Pour ce qui est la conception du script rien de plus simple.
- Premièrement, on lance le programme correspondant à la creation de liste à laquelle on veut accéder. Cette action permet de générer un fichier contenant la liste des paquets correspondant à la liste souhaité
- Deuxièmement, on affiche un message pour indiquer quelle liste est affichée, pour ensuite afficher le contenu de la liste avec la commande *cat*.
- Pour finir, pour une transparence la plus totale et éviter les fichiers inutile ou supprime avec la commmande *rm* le fichier contenant la liste précédement crée.