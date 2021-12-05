# Documentation du programme d'installation :

## Description générale
Le programme _lancer_ est un script bash s'occupant de l'installation du logiciel.
Lancé par l'administrateur, il permet d'integrer le logiciel à la machine Debian.

Ce programme modifie :
- /etc/sudoers         En ajoutant les droits d'installation de paquet à un utilisateur donné.
- /etc/bash.bashrc     En ajoutant la fonction _command\_not\_found\_handle()_ qui appelera le script bash _scriptInstallation_ pour installer un paquet
                       lorsque le paquet tapé dans la console n'est pas déjà installé.

## Description de la conception et réalisation du script

Au debut du script, on demande à l'administrateur de saisir le nom de l'utilisateur a qui il veut installer le logiciel.
Ce nom est nescessaire car on lui donnera les droits d'installation afin que le logiciel puisse installer des paquets sans demander le mot de passe administrateur a chaque fois. Il sera utilisé pour la modification du fichier _/etc/sudoers_.
On récupère ensuite dans une variable _chemin_ le chemin du script _lancer_, car on suppose qu'il a le même chemin que le script _scriptInstallation_. En effet, connaître le chemin de _scriptInstallation_ est nescessaire car on appelera ce script dans la fonction _command\_not\_found\_handle()_.

![alt text](./Image/Capture_lancer_1.png) 

On écrit ensuite dans le fichier /etc/sudoers la ligne suivante. Elle donnera à l'utilisateur dont le nom a été saisi plus tôt les droits d'installation de paquet.

![alt text](./Image/Capture_sudoers.png)

On écrit ensuite dans le fichier /etc/bash.bashrc la fonction suivante. Elle sera appellée lorsqu'une commande introuvable est inscrite dans la console. Elle essayera d'installer la un paquet qui a pour nom la commande introuvable en question. Si aucun paquet ne correspond au nom indiqué, alors elle marque sur la console _commande introuvable_. Mais si le paquet existe, alors elle l'installera et l'ouvrira automatiquement dès la fin de l'installation.

![alt text](./Image/Capture_command_not_found_handle.png)


On lit ensuite dans le fichier serec.config ( dont le chemin est ecrit pour le moment en dur dans le programme, mais ce sera problème disparaitra tout seul quand on incluera le serveur serecd) le chemin des listes rouge, verte et orange. Ce chemin est écrit part l'administrateur lors de l'installation du paquet.

On ajoute ensuite a la fin du fichier /home/"nom de l'utilisateur"/.bashrc une ligne permettant d'executer les commandes permettant d'acceder aux listes depuis n'importe quel endroit de la console.

![image](https://user-images.githubusercontent.com/81689403/144429286-7b3531a1-09ec-444e-a8e3-47f9bb1f3a2a.png)


### Ligne pour l'accès aux listes
![image d'accés aux listes](./Image/codeAccesListes.png)
Avec cette ligne de code, le but est d'ajouter dans le fichier ***.bashrc*** une ligne qui ajoute à la variable d'environement PATH le chemin qui mène aux scripts d'accès aux listes pour pouvoir y accéder de n'importe où dans le répertoire

### Gestion des erreurs
Le programme lancer présenté ci-dessus fonctionne parfaitement, s'il n'est lancé qu'une seule fois.
En effet, s'il est lancé plusieurs fois d'affilées, il écrit la fonction _command_not_found_handle()_ plusieurs fois dans le fichier _/etc/bash.bashrc_.
Ce problème est présent pour chacune des commandes rajoutées par le programme.
Pour gerer ça, nous verifions avant d'ajouter les commandes si elles ne sont pas déjà présentes.

Voici comment nous procedons pour la ligne rajoutée dans le fichier _/etc/sudoers_

![alt text](./Image/GestionErreurLancer.png)
