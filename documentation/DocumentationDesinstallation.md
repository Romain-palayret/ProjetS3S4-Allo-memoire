
# Loup Salomé 

## Fonction affichage des Dependances Inverses du paquet 

![alt text](Image2/imageProgC/AfficherDependanceInversePaquet.png)


Cette fonction prend en paramètre un paquet parent. A partir de ce paquet parent , 
la fonction va afficher toutes les rdepends du paquets , puis elle sera éxécutée
récursivement sur chaque paquet dépendant du paquet parent.
Ainsi , l’intégralité de l’arbre des dépendances est parcouru. 


## Fonction de dressage de la liste de suppression à partir d’un paquet parent : 

![alt text](Image2/imageProgC/dresserListePaquetSupprimer.png)


Cette fonction prend en parametre un un paquet parent , une tableau de chaine de caractère qui permettra
de stocker la liste des paquets à supprimer. L'algorithme sera le suivant : 

On parcourt l'arborescence de façon récursive de la même manière que l'on a fait pour l'affichage des dépendances
inverses. Sauf que cette fois ci , on ajoute le nom du paquet Parent à chaque appel Récursif. Si le paquet existe 
déjà dans la liste de Suppression , il ne sera pas ajouté. 


## Fonction création Paquet 

![alt text](Image2/imageProgC/CréationPaquet.png) 

La fonction création Paquet Fonctionne de la manière suivante. On passe en paramètre un nom de Paquet ainsi que le chemin Absolue du projet. On appelle 
ensuite le scriptBash RecupererPriorite.sh , qui créera un fichier priorite.txt dans le dossier scriptBash. La fonction va ensuite récupérer la priorité 
dans ce fichier texte. Si la priorité relevée est standard , required ou important. La fonction créerPaquet renverra null. Si cette fonction renvoit nulle , le programme
appelant se mettra en erreur et renverra un code d'erreur. 















