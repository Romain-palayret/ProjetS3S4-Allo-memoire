# Documentation du script ajoutUser :

## Description générale
Le programme _ajoutUser_ est un script bash permettant de modifier de manière simple le _serec.confg_.
Lancé par l'administrateur, il permet de changer le nombre de paquet Orange.

## Outils utilisés : 
Le script _lancer_ a été codé sur l'editeur texte _gedit_ et testé sur l'environnement Debian

## Description de la conception et réalisation du script

Au début du script, on récupère le chemin du fichier _serec.config_.

```
#On récupère le chemin de serec.config
cheminSerec='/etc/SeReC/serec.config'

```

On demande ensuite à l'administrateur de saisir le nom de l'utilisateur a qui il veut installer le logiciel.

```
#On demande de saisir le nom de l'utilisateur pour lequel on veut installer serec
echo "Veuillez saisir l'utilisateur : "
read nom
```

On demande ensuite à l'administrateur de choisir le profil de l'utilisateur.
En fonction de sa réponse, la liste Orange contiendra des paquets différents. Un gamer n'a pas les mêmes besoins qu'une secrétaire, donc les paquets à conserver et à désinstaller ne sont pas les mêmes.

```
# On demande le type de profil de l'utilisateur
echo ""
echo "=== CHOIX DU PROFIL ==="
echo "1) Administrateur"
echo "2) Secrétaire"
echo "3) Gamer"
echo "4) Mathématicien"
echo "5) Ingénieur Audiovisuel"

echo -n "Saisir le numéro du profil souhaité : ";
read numListe
```
Pour choisir l'utilisateur, l'administrateur dois écrire le chiffre correspondant : 1 pour un profil Administrateur, 2 pour un profil de Secrétaire...
On enregistre sa réponse dans une variable _numListe_.

On identifie le profil choisi à l'aide d'un switch : 

```
case $numListe in
        1) 
        sed -i '7d' $cheminSerec
        sed -i "7i\Administrateur : 1\t" $cheminSerec
        ;;
        2) 
        sed -i '8d' $cheminSerec
        sed -i "8i\Secretaire : 1\t" $cheminSerec
        ;;
        3) 
        sed -i '9d' $cheminSerec
        sed -i "9i\Gamer : 1\t" $cheminSerec
        ;;
        4)
        sed -i '10d' $cheminSerec
        sed -i "10i\Mathematicien : 1\t" $cheminSerec
        ;;
        5) 
        sed -i '11d' $cheminSerec
        sed -i "11i\Ingenieur Audio-Visuel : 1\t" $cheminSerec
        ;;
        *) echo "Erreur, veuillez choisir un chiffre entre 1 et 5"
           numListe=-1;;
esac
```

Pour creer la listeVerte, _creeListeVerte_ utilise le fichier serec.config pour savoir quelles listes Oranges doivent être prises en compte.
_creeListeVerte_ va lire la partie "_Liste des profils_" de serec.config.
```
Liste des profils :
Administrateur : 0
Secretaire : 0
Gamer : 0
Mathematicien : 0
Ingenieur Audio-visuel : 0

--Ne pas supprimer cette ligne--
```

Si le nombre à droite d'un des profils est 0, alors la liste Orange associée ne sera pas prise en compte.
Au contraire, si le nombre à droite des profils est 1, alors la liste Orange associée doit être prise en compte lors de la création de la ListeVerte.

Le rôle de _ajoutUser_ est de modifier le 0 du profil demandé par le chiffre 1.
Pour ce faire, il supprime entièrement la ligne du profil en question à l'aide de ```sed -i '7d' $cheminSerec``` (ici le chiffre ```'7d'``` signifie qu'on supprime la 7 ème ligne, celle de l'administrateur);
puis on réinsert la ligne mais en mettant un 1 à la place du 0 à l'aide de ```sed -i "7i\Administrateur : 1\t" $cheminSerec``` (ici le chiffre ```"7i\``` signifie qu'on insert au dessus de la 7 ème ligne);

La réinsertion d'une ligne se fait en précisant la ligne au dessus de laquelle on doit insérer. C'est pourquoi on de ne pas supprimer la dernière ligne du fichier, sans cette dernière la modification de la ligne de l'Ingénieur Audio-visuel est impossible.

On utilise une boucle while pour gérer le cas où l'administrateur ne rentrerait pas un chiffre entre 1 et 5 :
```
numListe=-1

while (($numListe == -1))
do 
    echo -n "Saisir le numéro du profil souhaité : ";
    read numListe
    
    case $numListe in
        1) 
        sed -i '7d' $cheminSerec
        sed -i "7i\Administrateur : 1\t" $cheminSerec
        ;;
        2) 
        sed -i '8d' $cheminSerec
        sed -i "8i\Secretaire : 1\t" $cheminSerec
        ;;
        3) 
        sed -i '9d' $cheminSerec
        sed -i "9i\Gamer : 1\t" $cheminSerec
        ;;
        4)
        sed -i '10d' $cheminSerec
        sed -i "10i\Mathematicien : 1\t" $cheminSerec
        ;;
        5) 
        sed -i '11d' $cheminSerec
        sed -i "11i\Ingenieur Audio-Visuel : 1\t" $cheminSerec
        ;;
        *) echo "Erreur, veuillez choisir un chiffre entre 1 et 5"
           numListe=-1;;
    esac
done
```
