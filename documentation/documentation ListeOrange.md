# Description générale
Une liste Orange est un fichier texte contenant les noms des paquets pouvant être désinstallé sans détruire la machine, mais qu'on décide de ne pas désinstaller afin de pouvoir utiliser la machine de de manière normale.
Par exemple, le paquet gnome contient l'interface graphique. L'interface graphique n'est pas nescessaire au fonctionnement de la machine, mais on ne la désinstalle pas car il est compliquer de travailler sans.

La principale caractéristique de la liste orange est qu'elle ne peut pas être écrite de manière exaustive, comme le peut l'être la liste rouge. En effet, un Administrateur n'a pas besoin d'interface graphique pour travailler, alors que c'est indispensable pour une secrétaire.
Nous avons donc décomposé la liste Orange en 6 listes, correspondant à 5 profils d'utilisateurs différents : 
 - Une Liste Orange par defaut, commune à tous les profils d'utilisateurs 
 - Une Liste Orange administrateur
 - Une Liste Orange secrétaire
 - Une Liste Orange gamer
 - Une Liste Orange mathématicien
 - Une Liste Orange ingénieur audio-visuel

Nous avons choisi ces profils car ils couvrent des besoins en paquets très différents.

# Processus de création :
Nous avons pris tous les paquets présents par défauts sur la machine Debian auquels nous avons retirés tous les paquets de la liste rouge.
Nous avons obtenu une liste de plus de 1300 paquets, que nous devions répartir dans les 6 listes Oranges.
Evidemment, la Liste Orange secrétaire peut avoir des paquets en communs avec la liste Orange du mathématicien.

Pour effectuer le tri, nous avons utilisé la commande dpkg --status, que nous avions déjà utilisée pour créer la liste rouge.
Cette commande donne une information utile : la section.
Tous les paquets sont répartis dans des sections. Il existe environ 60 sections différentes. Nous avons ensuite, a l'aide d'excel, attribuées à chacune de ces sections une ou plusieurs listeOrange. Ce document excel est disponible en annexe.

Une fois les sections triées, nous avons utilisée la commande grep pour dispacher les paquets dans les différentes listes Oranges en fonction de leur section. 
