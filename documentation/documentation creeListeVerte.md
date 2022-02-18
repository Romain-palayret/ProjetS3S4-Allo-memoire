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

_Voici un extrait de ce que produit la commande_ ```dpkg --status``` :
```
Package: zlib1g
Status: install ok installed
Priority: optional
Section: libs
Installed-Size: 166
Maintainer: Mark Brown <broonie@debian.org>
Architecture: amd64
Multi-Arch: same
Source: zlib
Version: 1:1.2.11.dfsg-2
Provides: libz1
Depends: libc6 (>= 2.14)
Breaks: libxml2 (<< 2.7.6.dfsg-2), texlive-binaries (<< 2009-12)
Conflicts: zlib1 (<= 1:1.0.4-7)
Description: compression library - runtime
 zlib is a library implementing the deflate compression method found
 in gzip and PKZIP.  This package includes the shared library.
Homepage: http://zlib.net/
```
_Ces info correspondent à un seul paquet, la commane "dpkg --status" affichera ces infos pour tous les paquets_

On connecte ensuite la sortie standart de cette commande à la commande ```grep -e "Priority: extra" -e "Priority: optional" -B2``` afin d'extraire du resultat de la commande précédente les lignes contenant les priorité des paquets "extra" et "optionnal". Cette commande à donc une double fonction : trier les paquets pour ne prendre que ceux qui nous intéressent (à savoir les paquets "extra" et "optionnal") mais aussi réduire le nombre d'informations fournies par de la commande ```dpkg --status``` pour ne garder que celles qui nous intéressent.

_Voici un extrait de ce que produit la commande ```dpkg --status | grep -e "Priority: extra" -e "Priority: optional" -B2```_ :
```
Package: yelp-xsl
Status: install ok installed
Priority: optional
--
Package: zenity
Status: install ok installed
Priority: optional
--
Package: zenity-common
Status: install ok installed
Priority: optional
--
Package: zlib1g
Status: install ok installed
Priority: optional
```

On connecte ensuite la sortie standart de cette commande à la commande ```sed -n '/Package/p'``` afin d'extraire les lignes contenant le nom des paquets.

_Voici un extrait de ce que produit la commande ```dpkg --status | grep -e "Priority: extra" -e "Priority: optional" -B2 | sed -n '/Package/p'```_ :
```
Package: unattended-upgrades
Package: uno-libs-private
Package: unoconv
Package: unzip
Package: update-inetd
Package: upower
Package: ure
Package: usb-modeswitch
Package: usb-modeswitch-data
Package: usb.ids
```

On connecte ensuite la sortie standart de cette commande à la commande ```sed -r 's/Package:\s(.*)/\1/'``` afin d'extraire le noms des paquets. 

_Voici un extrait de ce que produit la commande ```dpkg --status | grep -e "Priority: extra" -e "Priority: optional" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' ```_ :
```
lua-json
lua-lpeg
lua-socket
mailcap
malcontent
malcontent-gui
manpages-fr
media-player-info
mesa-va-drivers
mesa-vdpau-drivers
mesa-vulkan-drivers
mime-support
```
On stockera ensuite cette liste dans un fichier temporaire _ListeVerteTemp_.

### Fabrication de Listeverte

Le problème de la _ListeVerteTemp_ est qu'il ne s'agit que d'une liste des paquets "optional" et "extra" du système. Or certain de ces paquets sont aussi présents dans la _ListeRouge_  car ils sont utilisé par des paquets "important", "required" ou "standart" (par exemple le paquet _libc6_). Il faut donc enlever les paquets en communs entre la _ListeVerteTemp_ et la _ListeRouge_. Pource faire on utilise le code ci-dessous :
```
>ListeVerte          #on créé la liste verte
while read line     #on lit chaque paquet  de l'entrée standard
do
  if ! grep -q -m 1 "$line" ListeRouge  #si le paquet n'est pas au moins une fois dans ListeRouge
  then
    echo "$line" >>ListeVerte           #alors on ajoute le paquet à ListeVerte
  fi
done <ListeVerteTemp     #on envoie ListeVerteTemp sur l'entrée standard du while
```

On termine le script en supprimant la liste temporaire _ListeVerteTemp_.
```rm ListeVerteTemp```