# Documentation du programme de la création de la liste verte :

## Description générale
Le programme _creelisteVerte_ est un script bash qui permet de créer une liste de paquet pouvant être désinstallé sans risque pour l'intégrité de la machine.
C'est paquets ont la priorité "extra" ou la priorité "optionnal"
- Les paquets de priorités "extra" paquets qui entrent en conflit avec d'autres ayant des priorités plus élevées ou ayant des conditions particulières qui les rendent peu convenables pour être optionnels. Ils sont utiles seulement si vous savez déjà ce qu'ils font.
- Les paquets de priorités "optionnal" incluent tout ce que vous pourriez raisonnablement vouloir installer si vous ne savez pas ce que c'est ou n'avez pas de besoins spécialisés.

Pour creer la liste Verte, on considère que les paquets de la listes vertes sont : Tous les paquets "optionnal" ou "extra" moins les paquets de la liste rouge moins les paquets des listes Oranges actives.

### Avertissement

Ce script utilise un fichier temporaire. ce fichier nommé _ListeVerteTemp_ est crée au début du script et supprimé à la fin.
Il ne faut donc pas relancer ce script avant qu'il soit déjà fini.

### Temps d'execution

Environ 20 secondes.

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

Le problème de la _ListeVerteTemp_ est qu'il ne s'agit que d'une liste des paquets "optional" et "extra" du système. Or certain de ces paquets sont aussi présents dans la _ListeRouge_  car ils sont utilisé par des paquets "important", "required" ou "standart" (par exemple le paquet _libc6_). Il faut donc enlever les paquets en communs entre la _ListeVerteTemp_ et la _ListeRouge_. Pour ce faire on utilise le code ci-dessous :
```
creeListeRouge       #on créé la liste rouge a l'aide du script creeListeRouge

>ListeVerte          #on créé la liste verte
while read line     #on lit chaque paquet  de l'entrée standard
do
  if ! grep -q -m 1 "$line" ListeRouge  #si le paquet n'est pas au moins une fois dans ListeRouge
  then
    echo "$line" >>ListeVerte           #alors on ajoute le paquet à ListeVerte
  fi
done <ListeVerteTemp     #on envoie ListeVerteTemp sur l'entrée standard du while
```
Le code ci dessus ne suffit pas. En effet, il ne prends pas en compte les listes Oranges.
Les listes Oranges sont au nombre de 6 : une liste Orange par defaut (ListeOrangeDefaut) et 5 listes Oranges associées à des profils utilisateurs.

Pour prendre en compte la liste orange par défaut, c'est facile, il suffit de faire la même chose que pour la liste Rouge : 
```
creeListeRouge       #on créé la liste rouge a l'aide du script creeListeRouge

>ListeVerte          #on créé la liste verte
while read line     #on lit chaque paquet  de l'entrée standard
do
  if ! grep -q -m 1 "$line" ListeRouge  #si le paquet n'est pas au moins une fois dans ListeRouge
  then	
    if ! grep -q -m 1 "$line" /usr/etc/SeReC/SeReC_listes/ListeOrangeDefaut  #si le paquet n'est pas au moins une fois dans ListeOrangeDefaut
    then
        echo "$line" >>ListeVerte           #alors on ajoute le paquet à ListeVerte
    fi
  fi
done < ListeVerteTemp     #on envoie ListeVerteTemp sur l'entrée standard du while\nrm ListeVerteTemp\nrm ListeRouge
```

Par contre, prendre en compte les autres listes Oranges est plus complexe : ces dernières sont activables et désactivables en fonction des besoins des utilisateurs.
Pour ce faire, on utilise la fonction _chercherDansListe()_.

### Fonction _chercherDansListe()_ :
Permet de savoir si un paquet est contenu dans une des listes Oranges, et si cette liste Orange est activée ou non.

Cette fonction prends 3 arguments :
- Le nom du paquet à chercher
- le chemin de la liste Orange dans laquelle on veut chercher le paquet
- le numéro de la ligne du fichier _serec.config_ qui contient l'information qui permet de savoir si la liste est activée ou désactivée

Cette fonction retourn :
- 0 si le paquet n'est pas dans la liste ou si la liste est désactivée (donc que le paquet peut-être désinstallé)
- 1 si la liste Orange est activé et que le paquet s'y trouve (donc qu'on ne doit pas désinstaller le paquet)

```
function chercherDansListe() 
{
   paquet=$1
   cheminListe=$2
   typeProfil=$3
   
   var=$(sed -n $typeProfil'p' /etc/SeReC/serec.config)
   booleanProfil=${var##* }
   
   if [[ $booleanProfil == 0 ]]
   then
       return 0
   else
      if ! grep -q -m 1 "$1" "$2"
      then
        return 0    
      else
         return 1 
      fi
   fi   
}
```
On commence par extraire de _serec.config_ le chiffre (0 ou 1) indiquant si la liste Orange passée en argument est activée ou pas.
Pour ce faire on utilise les commandes ```var=$(sed -n $typeProfil'p' /etc/SeReC/serec.config)``` et ```booleanProfil=${var##* }``` qui permettent d'extraire le dernier mot d'une ligne donnée.

Si cette liste est désactivée, on return 0, sinon on vérifie si la liste Orange contient le paquet pris en argument, et si c'est le cas on return 1, sinon on return 0;


### Appels de la fonction _chercherDansListe()_
Pour savoir si on peut désinstaller un paquet, il faut 

```
creeListeRouge

>ListeVerte          #on créé la liste verte
while read line     #on lit chaque paquet  de l'entrée standard
do
  if ! grep -q -m 1 "$line" ListeRouge  #si le paquet n'est pas au moins une fois dans ListeRouge
  then	
    if ! grep -q -m 1 "$line" /usr/etc/SeReC/SeReC_listes/ListeOrangeDefaut
    then
        somme=0
        (chercherDansListe "$line" /etc/SeReC/SeReC_listes/ListeOrangeAdmin 7)
        somme=$(($somme + $?))
        (chercherDansListe "$line" /etc/SeReC/SeReC_listes/ListeOrangeSecretaire 8)
        somme=$(($somme + $?))
        (chercherDansListe "$line" /etc/SeReC/SeReC_listes/ListeOrangeGamer 9)
        somme=$(($somme + $?))
        (chercherDansListe "$line" /etc/SeReC/SeReC_listes/ListeOrangeMath 10)
        somme=$(($somme + $?))
        (chercherDansListe "$line" /etc/SeReC/SeReC_listes/ListeOrangeInge 11)
        somme=$(($somme + $?))

        if [[ $somme == 0 ]]
        then
            echo "$line" >>ListeVerte           #alors on ajoute le paquet à ListeVerte
        fi
    fi
  fi
done < ListeVerteTemp     #on envoie ListeVerteTemp sur l'entrée standard du while\nrm ListeVerteTemp\nrm ListeRouge
```
Pour savoir si on peut désinstaller un paquet, on appelle la fonction _chercherDansListe()_ pour chacune des listes orange. Si un seul de ces appels renvoie la valeur 1, alors on ne doit pas désinstaller le paquet. Pour vérifier cela, on fait la somme des valeurs de retour. Si cette somme est toujours égale à 0 à la fin des 5 appels, alors on peut ajouter le paquet à la liste Verte.


On termine le script en supprimant la liste temporaire _ListeVerteTemp_.
```rm ListeVerteTemp```
