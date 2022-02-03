# Documentation du programme de la création de la liste rouge :

## Description générale
Le programme _creeListeRouge_ est un script bash qui permet de créer une liste de paquet ayant la priorité _"important"_, _"required"_ ainsi que _"standard"_.  
- Les paquets de priorités _"important"_ sont paquets devant obligatoirement être présents sur tous systèmes de type Unix.
- Les paquets de priorité _"required"_ sont nécessaire au bon fonctionnement du système. La suppression d'un paquet avec cette priorité peut entrainer une panne et un dysfonctionnement partiel ou total du sytème. Parmis les risques possibles et fréquents, la désinstalation du paquet "dpkg" sinifie qu'il est impossible d'installer de nouveau paquet et ainsi ne pas pouvoir faire évoluer votre système. Mais maintenant avec notre nouvelle version elle prend aussi en compte toutes les dépendances et sous-dépendances du paquet en question.
- Les paquets de priorité _"standard"_ : Les paquets Standard sont présents sur n'importe quel système Linux, y compris un système raisonnablement petit mais pas limité à un système en mode console. Des outils sont disponibles pour envoyer des courriels (avec mutt) et télécharger des fichiers depuis des serveurs FTP. C'est ce qui sera installé par défaut si les utilisateurs ne sélectionnent rien. Cela n'inclut pas les grosses applications, mais intègre l'interpréteur Python et certains serveurs comme OpenSSH (pour l'administration à distance) et Exim (pour la livraison des courriels, même s'il peut être configuré en mode local uniquement). Sont également intégrées certaines documentations générales considérées utiles par la plupart des utilisateurs.

## Description de la conception et réalisation du script
Au début du script, on crée un fichier _file_ ainsi q'un fichier _listeRouge_. Par la suite, on redirige la sortie de commande _dpkg --status_ dans le fichier _file_. Après cela, on lit chaque ligne du fichier _file_. Lorsqu'une ligne contenant la chaîne "Package: " est trouvée on initialise une variable avec la valeurs 3 car il faut aller chercher la ligne ou l'on trouve la priortité du paquet touvé. Comme on peut le voir sur la capture d'écran suivante on retrouve en orange la ligne contenant la priortité du paquet.

```
apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends $(dpkg --status | grep -e "Priority: required" -e "Priority: important" -e "Priority: standard" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/') | grep -v ^[' '] | sort -u | xargs -L1 dpkg --status 2>/dev/null | grep ^["Priority:"].* -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' | grep -v ^[' '] > ListeRouge
```

Ensuite grace à la variable shell on découpe la ligne contenant le nom du paquet que l'on stock .Ensuite nous allons 3 lignes plus bas, c'est à dire la ligne contenant la priorité du paquet en question. Après cela on regarde si elle correspond à la priorité **important** ou **required**. Si c'est le cas on empile le nom du paquet dans le fichier _listeRouge_. A la fin du script, on supprime le fichier _file_ pour alléger le sytème.
