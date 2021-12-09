# Documentation du programme d'installation :

## Description générale
Le programme _lancer_ est un script bash s'occupant de l'installation du logiciel.
Lancé par l'administrateur, il permet d'integrer le logiciel à la machine Debian.

Ce programme modifie :
- /etc/sudoers         En ajoutant les droits d'installation et de désinstallation de paquet à un utilisateur donné.
- /etc/bash.bashrc     En ajoutant la fonction _command\_not\_found\_handle()_ qui appelera le script bash _scriptInstallation_ pour installer un paquet
                       lorsque le paquet tapé dans la console n'est pas déjà installé.
- /home/$nom/.bashrc   En ajoutant au PATH le chemin où sont stockée les listes pour pouvoir executées les listes n'importe où dans la console
- /etc/crontab         En ajoutant les lignes pour executer regulierment le programme de desinstallation

## Outils utilisés : 
Le script _lancer_ a été codé sur l'editeur texte _nano_ et testé sur l'environnement Debian

## Description de la conception et réalisation du script

Au début du script, on récupère le chemin du fichier _serec.config_. Ce chemin est pour le moment inscrit en dur dans le programme, et doit donc être modifié par l'intégrateur lors de l'installation de serec.config. Ce fichier contient diverse informations nescessaires au fonctionnement du script _lancer_, et doit donc être complet avant son execution.

```
#On récupère le chemin de serec.config
cheminSerec='/home/romain/Documents/ProjetS3/serec.config'
```

On demande ensuite à l'administrateur de saisir le nom de l'utilisateur a qui il veut installer le logiciel.
Ce nom est nescessaire car on lui donnera les droits d'installation afin que le logiciel puisse installer des paquets sans demander le mot de passe administrateur a chaque fois. Il sera utilisé pour la modification du fichier _/etc/sudoers_.
On récupère ensuite dans une variable _chemin_ le chemin du script _lancer_, car on suppose qu'il a le même chemin que le script _scriptInstallation_. En effet, connaître le chemin de _scriptInstallation_ est nescessaire car on appelera ce script dans la fonction _command\_not\_found\_handle()_.

```
#On demande de saisir le nom de l'utilisateur pour lequel on veut installer serec
echo "Veuillez saisir l'utilisateur : "
read nom
#Le fichier lancer doit être executer dans son dossier, et pas en dehors
chemin=$(pwd)
```
La suite du programme est découpée en 5 partie. Ces parties correspondent à chaque modifications de fichier qu'effectue le script _lancer_ et donc à chaque fonctionnalités que ces modifications permettent.

### Command _command\_not\_found\_handle()_ :

Le morceau de code suivant permet d'ecrire la fonction _command\_not\_found\_handle()_ dans le fichier /etc/bash.bashrc. Cette fonction sera appellée lorsqu'une commande introuvable est inscrite dans la console. Elle essayera d'installer la un paquet qui a pour nom la commande introuvable en question. Si aucun paquet ne correspond au nom indiqué, alors elle marque sur la console _commande introuvable_. Mais si le paquet existe, alors elle l'installera et l'ouvrira automatiquement dès la fin de l'installation.

```
echo -e "command_not_found_handle() {\n    sudo $chemin/scriptInstallation \"\$1\";\n    if [ -e /usr/bin/\"\$1\" ]\n    then\n        \"\$1\";\n    else\n        echo \"commande introuvable\";\n    fi\n    return 127;\n}" | sudo tee -a /etc/bash.bashrc
```
Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _lancer_ était executé plusieurs fois (pour définir plusieurs utilisateurs par exemple, ou par simple erreur humaine) La fonction _command\_not\_found\_handle()_ était écrite plusieurs fois. Si cette fonction est écrite plusieurs fois, alors elle n'est plus appelée par le système. Pour regler ce problème, nous allons ecrire le contenu de _/etc/bash.bashrc_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ```command_not_found_handle() {``` est déjà écrite. Si c'est le cas, la fonction ne sera pas réécrite.

```
#On ecrit la fonction command_not_found_handle si elle n'existe pas déjà
cat /etc/bash.bashrc > bashrc
dejaEcrit=0
while read ligne
do
    if [[ $ligne == "command_not_found_handle() {" ]]
    then
        dejaEcrit=1
    fi
done < bashrc

if [ $dejaEcrit -eq "0" ]
then
echo -e "command_not_found_handle() {\n    sudo $chemin/scriptInstallation \"\$1\";\n    if [ -e /usr/bin/\"\$1\" ]\n    then\n        \"\$1\";\n    else\n        echo \"commande introuvable\";\n    fi\n    return 127;\n}" | sudo tee -a /etc/bash.bashrc
fi
rm bashrc
```

### Droits d'installations des paquets :

On écrit ensuite dans le fichier /etc/sudoers la ligne suivante. Elle donnera à l'utilisateur dont le nom a été saisi plus tôt les droits d'installation de paquet.
```echo "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptInstallation" | sudo tee -a /etc/sudoers```

Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _lancer_ était executé plusieurs fois (pour définir plusieurs utilisateurs par exemple, ou par simple erreur humaine) La ligne de code ci-dessus était écrite plusieurs fois. Ecrire cette ligne une seconde fois peut-être nescessaire si le nom d'utilisateur est différent, mais si le nom d'utilisateur est le même cette réécriture n'est pas souhaitée. Pour regler ce problème, nous allons ecrire le contenu de _/etc/sudoers_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ci-dessus est déjà écrite. Si c'est le cas, la ligne ne sera pas réécrite.

```
#On ecrit la commande pour donner les droit d installation dans le fichier /etc/sudoers si elle n'existe pas déjà
cat /etc/sudoers > sudoer
dejaEcrit=0
while read ligne
do
    if [[ $ligne == "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptInstallation" ]]
    then
        dejaEcrit=1
    fi
done < sudoer

if [ $dejaEcrit -eq "0" ]
then
    echo "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptInstallation" | sudo tee -a /etc/sudoers
fi
rm sudoer

```

### Droits de désinstallation des paquets :

On écrit ensuite dans le fichier /etc/sudoers la ligne suivante. Elle donnera à l'utilisateur dont le nom a été saisi plus tôt les droits d'installation de paquet.
```echo "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptDesinstallation" | sudo tee -a /etc/sudoers```

Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _lancer_ était executé plusieurs fois (pour définir plusieurs utilisateurs par exemple, ou par simple erreur humaine) La ligne de code ci-dessus était écrite plusieurs fois. Ecrire cette ligne une seconde fois peut-être nescessaire si le nom d'utilisateur est différent, mais si le nom d'utilisateur est le même cette réécriture n'est pas souhaitée. Pour regler ce problème, nous allons ecrire le contenu de _/etc/sudoers_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ci-dessus est déjà écrite. Si c'est le cas, la ligne ne sera pas réécrite.

```
#On ecrit la commande pour donner les droit de desinstallation dans le fichier /etc/sudoers si elle n'existe pas déjà
cat /etc/sudoers > sudoer
dejaEcrit=0
while read ligne
do
    if [[ $ligne == "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptDesinstallation" ]]
    then
        dejaEcrit=1
    fi
done < sudoer

if [ $dejaEcrit -eq "0" ]
then
    echo "$nom ALL=(ALL:ALL) NOPASSWD: $chemin/scriptDesinstallation" | sudo tee -a /etc/sudoers
fi
rm sudoer

```
### Ajout au PATH du chemin des listes :

On commence extraire du fichier serec.config le chemin des listes rouge, verte et orange.
```
var=$(sed -n '2p' $cheminSerec)
cheminListe=${var##* }
```
On ajoute ensuite à la fin du fichier /home/$nom/.bashrc la ligne de code ```export PATH=\"\$PATH:$cheminListe\"``` . Cette ligne permet d'executer les commandes permettant d'acceder aux listes depuis n'importe quel endroit de la console.

```echo "export PATH=\"\$PATH:$cheminListe\"" >> /home/$nom/.bashrc```

Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _lancer_ était executé plusieurs fois (pour définir plusieurs utilisateurs par exemple, ou par simple erreur humaine) La ligne de code ci-dessus était écrite plusieurs fois. Ecrire cette ligne une seconde fois peut-être nescessaire si le nom d'utilisateur est différent (car le fichier d'écriture est différent), mais si le nom d'utilisateur est le même cette réécriture n'est pas souhaitée car elle conduit à des comportements imprévisibles. Pour regler ce problème, nous allons ecrire le contenu de _/home/$nom/.bashrc_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ci-dessus est déjà écrite. Si c'est le cas, la ligne ne sera pas réécrite.

```
# On ajoute au PATH le chemin où sont stockée les listes pour pouvoir executées
# les listes n'importe où dans la console
var=$(sed -n '2p' $cheminSerec)
cheminListe=${var##* }

#On ajoute la ligne si elle n'est pas déjà ecrite
cat /home/$nom/.bashrc > lebashrc
dejaEcrit=0
while read ligne
do
    if [[ $ligne == "export PATH=\"\$PATH:$cheminListe\"" ]]
    then
        dejaEcrit=1
    fi
done < lebashrc

if [ $dejaEcrit -eq "0" ]
then
    echo "export PATH=\"\$PATH:$cheminListe\"" >> /home/$nom/.bashrc
fi
rm lebashrc
```
### Ajout au _crontab_ de l'execution du script dReccur :

On commence extraire du fichier serec.config le nombre de secondes avant lesquelles un paquet non utilisé doit être désinstallé.
Lorsque l'intégrateur modifie le fichier _serec.config_, on recommande à l'intégrateur de saisir un nombre de secondes supérieur à 7200, car le programme de désinstallation _dReccur_ est assez long, et ce chiffre assez élevé permet de ne pas executer le programme _dReccur_ avant que la précédente execution soit terminée.


```
# On récupère le nombre de seconde après la désinstallation
var=$(sed -n '1p' $cheminSerec)
secondeDes=${var##* }
# On ajoute dans une variable le chemin où est le script dReccur
var=$(sed -n '5p' $cheminSerec)
chemindReccur=${var##* }
```

On ajoute ensuite à la fin du fichier /etc/crontab la ligne de code ```*/$(($secondeDes/60)) * * * * root bash ..$chemindReccur/dReccur``` . Cette ligne permet d'executer toutes les _$secondesDes_ (variable qui contient les secondes extraites précédemment dans le fichier _serec.config_) le script _dReccur_ qui gere la désinstallation des paquets.

```echo "*/$(($secondeDes/60)) * * * * root bash ..$chemindReccur/dReccur" >> /etc/crontab```

Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _lancer_ était executé plusieurs fois (pour définir plusieurs utilisateurs par exemple, ou par simple erreur humaine) la ligne de code ci-dessus était écrite plusieurs fois. Cela pose problème car si la ligne est écrite 2 fois, le script _dReccur_ sera executer 2 fois simultanement.  Pour regler ce problème, nous allons ecrire le contenu de _/etc/crontab_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ci-dessus est déjà écrite. Si c'est le cas, la ligne ne sera pas réécrite.

```
# On ajoute la ligne qui correspond au lancement du script dReccur toutes les X secondes
cat /etc/crontab > crontabText

nbLigne=0
while read ligne
do
    ((nbLigne++))	
    if [[ $ligne == *"..$chemindReccur/dReccur" ]]
    then
        sed -i.bak $nbLigne'd' /etc/crontab
    fi
done < crontabText

echo "*/$(($secondeDes/60)) * * * * root bash ..$chemindReccur/dReccur" >> /etc/crontab


rm crontabText
```

