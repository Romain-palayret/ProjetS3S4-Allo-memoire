# Documentation du programme d'installation :

## Description générale
Le programme _postint_ est un script bash s'occupant de l'installation du logiciel.
Lancé automatiquement après l'installation du paquet SeReC, il permet d'integrer le logiciel à la machine Debian.

Ce programme modifie :
- /etc/bash.bashrc     En ajoutant la fonction _command\_not\_found\_handle()_ qui appelera le script bash _scriptInstallation_ pour installer un paquet
                       lorsque le paquet tapé dans la console n'est pas déjà installé.
- /home/$nom/.bashrc   En ajoutant au PATH le chemin où sont stockée les listes pour pouvoir executées les listes n'importe où dans la console
- /etc/rc.local        Pour lancer les serveurs automatiquement au démarage de la machine.

## Outils utilisés : 
Le script _postint_ a été codé sur l'editeur texte _vim_ et testé sur l'environnement Debian

## Description de la conception et réalisation du script

Au début du script, on récupère le chemin du fichier _serec.config_. Ce chemin est inscrit en dur dans le programme, car serec.config sera toujours au même endroit quelque soit la machine. Ce fichier contient diverse informations nescessaires au fonctionnement du script _postint_. A l'installation du paquet, _serec.config_ contient des valeurs par defauts, mais elles peuvent être modifiée à tout moment par l'administrateur.

```
#On récupère le chemin de serec.config
cheminSerec='/usr/etc/SeReC/serec.config'
```


On récupère ensuite dans une variable _chemin_ le chemin des différents scripts de l'application. En effet, connaître le chemin de _client.exe_ est nescessaire car on appelera ce script dans la fonction _command\_not\_found\_handle()_. Ce chemin est écrit en dur car il ne change pas en fonction des machines.

```
##On recupere le chemin des scripts
chemin='/usr/bin/SeReC'
```
La suite du programme est découpée en 5 partie. Ces parties correspondent à chaque modifications de fichier qu'effectue le script _lancer_ et donc à chaque fonctionnalités que ces modifications permettent.

### Command _command\_not\_found\_handle()_ :

Le morceau de code suivant permet d'ecrire la fonction _command\_not\_found\_handle()_ dans le fichier /etc/bash.bashrc. Cette fonction sera appellée lorsqu'une commande introuvable est inscrite dans la console. Elle essayera d'installer la un paquet qui a pour nom la commande introuvable en question. Si aucun paquet ne correspond au nom indiqué, alors elle marque sur la console _commande introuvable_. Mais si le paquet existe, alors elle l'installera et l'ouvrira automatiquement dès la fin de l'installation. 

Un simple utilisateur n'a pas les droits d'installation de paquet. Pour contourner le problème, la fonction _command\_not\_found\_handle()_ va executer _client.exe_ qui va demander à un serveur lancé avec les droits d'administrateur (le serveur _serveur.exe_) d'installer le paquet en question à sa place.

```
echo -e "command_not_found_handle() {\n    $chemin/SeReC/client.exe \"\$1\";\n    if [ -e /usr/bin/\"\$1\" ]\n    then\n        \"\$1\";\n    else\n        echo \"commande introuvable\";\n    fi\n    return 127;\n}" | sudo tee -a /etc/bash.bashrc
```
Au cours du projet, nous nous sommes rendu compte que cette simple commande était insuffisante. En effet, si le script _postint_ était executé plusieurs fois. La fonction _command\_not\_found\_handle()_ était écrite plusieurs fois. Si cette fonction est écrite plusieurs fois, alors elle n'est plus appelée par le système. Pour regler ce problème, nous allons ecrire le contenu de _/etc/bash.bashrc_ dans un fichier temporaire, afin de le parcourir plus facilement. On vérifie ensuite si la ligne de code ```command_not_found_handle() {``` est déjà écrite. Si c'est le cas, la fonction ne sera pas réécrite.

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
echo -e "command_not_found_handle() {\n    $chemin/SeReC/client.exe \"\$1\";\n    if [ -e /usr/bin/\"\$1\" ]\n    then\n        \"\$1\";\n    else\n        echo \"commande introuvable\";\n    fi\n    return 127;\n}" | sudo tee -a /etc/bash.bashrc
fi
rm bashrc
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
### Creation _/etc/rc.local_ et lancement automatique du serveur :

Sur la dernière version de Debian, le fichier _/etc/rc.local_ n'existe pas. C'est pourquoi on doit le recreer de 0.
```
> /etc/rc.local
```

Pour fonctionner, ce fichier doit respecter un certain format : il doit commencer avec des lignes précise en commentaire, et la dernière ligne doit être un ```exit 0```. 
Pour lancer un programme au démarage de la machine, une ligne ```chemin/script > /dev/null 2&1 &``` doit être ajoutée juste avant le ```exit 0```.
Pour gagner du temps, on écrit tout d'un coup à l'aide d'un _echo -e_ :
```
echo -e "#!/bin/sh -e\n#\n# rc.local\n#\n# This script is executed at the end of each multiuser runlevel.\n# Make sure that the script will "exit 0" on success or any other\n# value on error.\n#\n# In order to enable or disable this script just change the execution\n# bits.\n#\n# By default this script does nothing.\n/usr/bin/SeReC/serveur.exe > /dev/null 2&1 &\n/usr/bin/SeReC/serveurDesinstallation.exe > /dev/null 2&1 &\nexit 0" | sudo tee -a /etc/rc.local
```

On termine en donnant les droit d'execution :
```
chmod +x /etc/rc.local
```
