# Documentation du programme dReccur :

## Description générale
Le programme _dReccur_ est un script bash s'occupant de trier quel paquet n'a pas été utilisé depuis longtemps, puis d'appeller une fonction de désinstallation pour le désinstaller si la désinstallation de celui ci ne présente aucun risque pour la machine Debian.
Ce script sera executé à intervalles réguliers sans l'intervention de l'utilisateur.

## Description de la conception et réalisation du script
On commence par récupérer dans le fichier _serec.config_ le temps en seconde au bout duquel un paquet doit être desinstaller s'il n'est pas utilisé.
Ce temps est le dernier nombre de la première ligne du fichier _serec.config_.
On le stock dans une variable _temps_.

```
# On récupère le temps de désinstallation défini dans serec.config
var=$(sed -n '1p' $cheminSerec)
temps=${var##* }
```

On extrait ensuite du fichier _serec.config_ le chemin du paquet. Ce chemin est nescessaire pour appeller les autres script du paquet.

```
var=$(sed -n '4p' $cheminSerec)
lechemin=${var##* }
```

On extrait ensuite du fichier _serec.config_ le chemin des listes. Ce chemin est nescessaire pour pouvoir lire la liste verte et savoir quel paquet on peut désinstaller sans risque.

```
var=$(sed -n '2p' $cheminSerec)
cheminListe=${var##* }
```

Dans le fichier listeVerte sont stocké les noms des paquets qui ne sont pas nescessaire au fonctionnement de la machine Debian.
Chaque ligne de ce fichier est un nom de paquet.
On parcourt donc ce fichier dans une boucle while et, avec la commande find, on stock dans un fichier temporaire _heureAcces_ l'ensemble des dernières heures d'acces de chaque fichier de _usr_ se terminant par le nom du paquet en question.

```
while read paquet
do
    find /usr -name $paquet -printf "%AH:%AM %Aj %AY\n" > heureAcces
```

![alt text](./Image/contenuHeureAcces_serecConfig.png) voici le contenu du fichier temporaire _heureAcces_ la commande find pour le paquet _gimp_.

On est parti du principe que si au moins un de ces fichiers à été utilisé recemment, alors le paquet en question ne doit pas être supprimé.
Une fois que cette liste d'heure est récupérée dans le fichier, on doit vérifier si toutes ces dates sont anciennes.
Pour ce faire, on sépare dans des variables l'heure, les minutes, les jours depuis le début de l'année et l'année en cours. Puis on convertit tout en seconde.

```
    heureCourante=$(date +%k)
    minuteCourante=$(date +%M)
    jourCourant=$(date +%j)
    anneeCourante=$(($(date +%Y)-2000))

    secondeCourante=$(((10#$minuteCourante*60)+($heureCourante*3600)+(10#$jourCourant*86400)+(10#$anneeCourante*31536000)))
```

Dans une boucle while qui parcourt le fichier temporaire, vérifie si cette heure est récente ou non.
Si cette heure est trop récente, inutile de continuer : on ne doit pas désinstaller le paquet.
Pour déterminer si l'heure est récente ou non, on la compare avec l'heure courante (elle aussi convertit en seconde) auquel on soustrait le temps contenu dans le fichier _serec.config_.

```
    while read heure
    do
        #On extrait les infos de la ligne
        heureLigne=${heure:0:2}
        minuteLigne=${heure:3:2}
        jourLigne=${heure:6:3}
        anneeLigne=${heure:12:2}

        #On convertit tout ça en seconde
        secondeLigne=$(((10#$minuteLigne*60)+(10#$heureLigne*3600)+(10#$jourLigne*86400)+(10#$anneeLigne*31536000)))

        #on compare les dernières dates d'execution du fichier avec la date courante pour savoir si on doit le supprimer
        if [ $(($secondeCourante-$temps)) -lt $secondeLigne  ]
        then
            nePasSup=1 #le paquet ne doit pas être supprimer
            break
        fi

    done < heureAcces
```
 
Si le paquet n'a pas été utilisé depuis longtemps, on appelle la fonction de desinstallation qui se chargera de désinstaller le paquet et mettre à jour le log.

```
    #si le paquet ne doit pas être supprimer on ne rentrera pas dans le if
    if [ $nePasSup -eq 0 ]
    then
        #desinstaller le paquet avec le programme de Loup
        $lechemin/programmeC/exe $paquet $lechemin/desinstallation >/dev/null 2>&1
        if [ $? -eq 0 ]
        then
            sudo $lechemin/scriptDesinstallation $paquet

        fi
    fi
```
