# Documentation du programme de la création de la liste verte :

## Description générale
Le programme _creelisterouge_ est un script bash qui permet de créer une liste de paquet pouvant être désinstallé sans risque pour l'intégrité de la machine.
C'est paquets ont la priorité "extra" ou la priorité "optionnal"
- Les paquets de priorités "extra" paquets qui entrent en conflit avec d'autres ayant des priorités plus élevées ou ayant des conditions particulières qui les rendent peu convenables pour être optionnels. Ils sont utiles seulement si vous savez déjà ce qu'ils font.
- Les paquets de priorités "optionnal" incluent tout ce que vous pourriez raisonnablement vouloir installer si vous ne savez pas ce que c'est ou n'avez pas de besoins spécialisés.

### Avertissement

Ce script utilise un fichier temporaire. ce fichier nommé _ListeVerteTemp_ est crée au début du script et supprimé à la fin.
Il ne faut donc pas relancer ce script avant qu'il soit déjà fini.

### Temps d'execution

Environ 1 seconde.

## Description de la conception et réalisation du script

Ce script est décomposé en 2 étapes : 
- Fabrication d'une liste contenant tous les paquets "optionnal" et "required" du système (désignée plus bas _ListeVerteTemp_)
- Suppression des paquets en commun avec la liste Rouge et la liste Orange

### Fabrication de la _ListeVerteTemp_

Une seule ligne de code est nescessaire à cette étape :

```dpkg --status | grep -e "Priority: extra" -e "Priority: optional" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' > ListeVerteTemp```

On commence tout d'abord par faire la commande ```dpkg --status``` qui va afficher les informations de tous les paquets du systèmes.

_Voici ce que produit la commande_ :


On connecte ensuite la sortie standart de cette commande à la commande ```grep -e "Priority: extra" -e "Priority: optional" -B2``` afin d'extraire du resultat de la commande précédente les lignes contenant les priorité des paquets "extra" et "optionnal". Cette commande à donc une double fonction : trier les paquets pour ne prendre que ceux qui nous intéressent (à savoir les paquets "extra" et "optionnal") mais aussi d'extraire de la commande ```dpkg --status``` les lignes contenant le nom des paquets.

_Voici ce que produit la commande ```dpkg --status | grep -e "Priority: extra" -e "Priority: optional" -B2```_ :
