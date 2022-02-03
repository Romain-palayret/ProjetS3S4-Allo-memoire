# Documentation du programme de la création de la liste rouge :

## Description générale
Le programme _cree\_liste\_rouge_ est un script bash qui permet de créer une liste de paquet contenant la priorité "important" et la priorité "required" ainsi que la priorité "standard".  
- Les paquets de priorités "important" sont paquets devant obligatoirement être présents sur tous systèmes de type Unix.
- Les paquets de priorités "required" sont nécessaire au bon fonctionnement du système. La suppression d'un paquet avec cette priorité peut entrainer une panne et un dysfonctionnement partiel ou total du sytème. Parmis les risques possibles et fréquents, la désinstalation du paquet "dpkg" sinifie qu'il est impossible d'installer de nouveau paquet et ainsi ne pas pouvoir faire évoluer votre système. Mais maintenant avec notre nouvelle version elle prend aussi en compte toutes les dépendances et sous-dépendances du paquet en question.

## Description de la conception et réalisation du script
Au début du script, on crée un fichier _file_ ainsi q'un fichier _listeRouge_. Par la suite, on redirige la sortie de commande _dpkg --status_ dans le fichier _file_. Après cela, on lit chaque ligne du fichier _file_. Lorsqu'une ligne contenant la chaîne "Package: " est trouvée on initialise une variable avec la valeurs 3 car il faut aller chercher la ligne ou l'on trouve la priortité du paquet touvé. Comme on peut le voir sur la capture d'écran suivante on retrouve en orange la ligne contenant la priortité du paquet.

```
apt-cache --no-breaks --no-conflicts --no-suggests --no-breaks --no-replaces --no-recommends --no-enhances --recurse depends $(dpkg --status | grep -e "Priority: required" -e "Priority: important" -e "Priority: standard" -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/') | grep -v ^[' '] | sort -u | xargs -L1 dpkg --status 2>/dev/null | grep ^["Priority:"].* -B2 | sed -n '/Package/p' | sed -r 's/Package:\s(.*)/\1/' | grep -v ^[' '] > ListeRouge
```

Ensuite grace à la variable shell on découpe la ligne contenant le nom du paquet que l'on stock .Ensuite nous allons 3 lignes plus bas, c'est à dire la ligne contenant la priorité du paquet en question. Après cela on regarde si elle correspond à la priorité **important** ou **required**. Si c'est le cas on empile le nom du paquet dans le fichier _listeRouge_. A la fin du script, on supprime le fichier _file_ pour alléger le sytème.
