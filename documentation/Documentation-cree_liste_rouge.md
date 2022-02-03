# Documentation du programme de la création de la liste rouge :

## Description générale
Le programme _creeListeRouge_ est un script bash qui permet de créer une liste de paquet ayant la priorité _"important"_, _"required"_ ainsi que _"standard"_.  
- Les paquets de priorités _"important"_ sont paquets devant obligatoirement être présents sur tous systèmes de type Unix.
- Les paquets de priorité _"required"_ sont nécessaire au bon fonctionnement du système. La suppression d'un paquet avec cette priorité peut entrainer une panne et un dysfonctionnement partiel ou total du sytème. Parmis les risques possibles et fréquents, la désinstalation du paquet "dpkg" sinifie qu'il est impossible d'installer de nouveau paquet et ainsi ne pas pouvoir faire évoluer votre système. Mais maintenant avec notre nouvelle version elle prend aussi en compte toutes les dépendances et sous-dépendances du paquet en question.
- Les paquets de priorité _"standard"_ : Les paquets Standard sont présents sur n'importe quel système Linux, y compris un système raisonnablement petit mais pas limité à un système en mode console. Des outils sont disponibles pour envoyer des courriels (avec mutt) et télécharger des fichiers depuis des serveurs FTP. C'est ce qui sera installé par défaut si les utilisateurs ne sélectionnent rien. Cela n'inclut pas les grosses applications, mais intègre l'interpréteur Python et certains serveurs comme OpenSSH (pour l'administration à distance) et Exim (pour la livraison des courriels, même s'il peut être configuré en mode local uniquement). Sont également intégrées certaines documentations générales considérées utiles par la plupart des utilisateurs.

## Description de la conception et réalisation du script
Au début de script on va chercher de manière récursive les dépendances des paquets ayant les priorité important, required ou encore standard de maniere récursive en supprimant de nos recherches les paquets breaks, conflits, suggests, breaks, replaces, recommends, enhances. En fait nous avons réécrit une commande du nom de apt-rdepends. Avec les grep on cible les paquets ayant les priorités que l'on recherche pour faire notre fonctionner notre algorithme récursif dessus. 

```
apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends $(dpkg --status | grep -e "Priority: required" -e "Priority: important" -e "Priority: standard" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/') | grep -v ^[' '] | sort -u | xargs -L1 dpkg --status 2>/dev/null | grep ^["Priority:"].* -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' | grep -v ^[' '] > ListeRouge
```

#Comparaison notre script à apt-rdepends : 

##Extrait final du résultat de creeListeRouge :

libbrotli1
libbsd0
libbz2-1.0
libc6
libcap2
libcap2-bin
libcap-ng0
libc-bin
libcbor0
libc-l10n
libcom-err2
libcrypt1
libcryptsetup12
libcurl3-gnutls


##cat ListeRouge | wc -l  : 

254

##apt-redepends libc6 : 

Reading package lists... Done
Building dependency tree... Done
Reading state information... Done
libc6
  Depends: libcrypt1
  Depends: libgcc-s1
libcrypt1
  Depends: libc6 (>= 2.25)
libgcc-s1
  Depends: gcc-10-base (= 10.2.1-6)
  Depends: libc6 (>= 2.14)
gcc-10-base


##notre script qui remplace apt-rdepends : seule difference c'est pas trié dans l'ordre alphabétique : 

###code :

apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends libc6

###resultat :

libc6
  Dépend: libgcc-s1
  Dépend: libcrypt1
libgcc-s1
  Dépend: gcc-10-base
  Dépend: libc6
libcrypt1
  Dépend: libc6
gcc-10-base

##Ensuite on applique notre script en ciblant les paquets required, important et standard : pour les avoirs avec leurs dépendances : 

###code :

apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends $(dpkg --status | grep -e "Priority: required" -e "Priority: important" -e "Priority: standard" -B2 )

###resultat :

libmpfr6
  Dépend: libc6
  Dépend: libgmp10
libsigsegv2
  Dépend: libc6
libbrotli1
  Dépend: libc6
libldap-2.4-2
  Dépend: libc6
  Dépend: libgnutls30
  Dépend: libsasl2-2
libnghttp2-14
  Dépend: libc6
librtmp1
  Dépend: libc6
  Dépend: libgmp10
  Dépend: libgnutls30
  Dépend: libhogweed6
  Dépend: libnettle8
  Dépend: zlib1g
libssh2-1
  Dépend: libc6
  Dépend: libgcrypt20
  Dépend: zlib1g
<python3:any>
<debconf-2.0>
<perlapi-5.32.0>

#Voici le script final : 

apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends $(dpkg --status | grep -e "Priority: required" -e "Priority: important" -e "Priority: standard" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/') | grep -v ^[' '] | sort -u | xargs -L1 dpkg --status 2>/dev/null | grep ^["Priority:"].* -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' | grep -v ^[' '] > ListeRouge

sed -n '/Package/p' : il va cibler la ligne ou y'a ecrit package
ed -r 's/Package:\s(.*)/\1/') : decoupe la chaine pour ne recuperer que le nom du pacquet

grep -v ^[' '] : va permettre de supprimer les lignes ou un espace apparait donc les éléments que l'on souhaite écarté

sort -u : va permettre de trier la liste obtenu dans l'ordre numerique et alphabétique

xargs -L1 dpkg --status 2>/dev/null : enlever les message d'erreur due aux paquet qui ne sont pas installés.

On ecrit finalement le résultat de nos recherches dans un fichier liste rouge que l'on crée.














