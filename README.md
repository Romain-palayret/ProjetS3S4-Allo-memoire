# ProjetS3S4-Allo-memoire
Projet durant le semestre S3 et S4 en collaboration avec l'IUT de Rodez.  
Ce projet permet l'allocution dynamique de la masse logiciel d'un système GNU/Linux  
# Auteur :
**PALAYRET** Romain  
**DEPRECQ** Corentin  
**HELG** Guillaume  
**SALOME** Loup  

# Date clés
Date de début de projet                        : 12 octobre 2021  
Date de rendu de la première version du projet : 10 décembre 2021

# Description générale du projet 

**Ajustement dynamique de la masse logicielle d'un système GNU/Linux**

Les systèmes d'exploitation et utilitaires associés présents dans les systèmes informatiques d'aujourd'hui 
sont de plus en plus volumineux. Pourtant, seule une faible partie de ces utilitaires est effectivement utilisée 
par les usagers. Cela a pour conséquences : 

- de nécessiter des disques de taille de plus en plus importante 
- de gaspiller le temps administrateur système en sauvegardes et contrôles d'intégrité d'exécutables peu ou pas utilisés. 


L'objet du projet SeReC est l'ajustement dynamique du nombre d'exécutables présents dans une distribution GNU/Linux 
Debian suivant l'utilisation réelle du système par les usagers. Cet ajustement doit être aussi transparent que possible 
à l'utilisateur tout en offrant des performances acceptables, Le gain attendu est une occupation minimale de l'espace 
disque occupé par la couche utilitaires du système informatique cible. 


La solution envisagée consiste à structurer l'application SeReC en deux parties : 
Une partie client qui exécute la commande utilisateur demandée si elle est localement présente, sinon demande son 
téléchargement / installation à un serveur local serecd puis l'exécute réellement. Cette partie client se matérialisera sous 
la forme d'une fonction shell prédéfinie dans bash (command not found_handle) dont il faudra écrire le code. Cette fonction 
est appelée par bash lorsque la commande à exécuter est introuvable. 


Un serveur serecd local à la machine cliente qui prend en charge les requêtes des processus bash interactifs, demande à un 
dépôt distant le téléchargement des paquetages requis, les installe puis exécute la commande demandée. C'est le serveur 
serecd qui désinstalle régulièrement les paquetages non utilisés depuis un certain temps. Pour cela, il gère trois listes 
de paquetages. La liste rouge contient les paquetages obligatoires pour un fonctionnement correct d'un système GNU/Linux 
Debian : ils ne peuvent être désinstallés automatiquement par serecd. La liste orange est gérée par l'administrateur du 
système et contient les paquetages qui ne doivent pas être désinstallés pour diverses raisons (configuration délicate, 
applications principales du système, etc.). Enfin, la liste verte contient tous les autres paquetages qui sont automatiquement 
installés ou désinstallés par serecd. 


L'administrateur du système devra disposer 
- D'un utilitaire cree_liste_rouge permettant de créer automatiquement à partir d'un distribution GNU/Linux Debian donnée 
la liste rouge des paquetages interdits de désinstallation 
- D'un fichier de configuration serec.config permettant de fixer la période entre deux désinstallations automatiques 
effectuées par serecd, la localisation des listes, etc.


**Les principaux travaux à réaliser sont les suivants :**
- L'écriture d'un document récapitulant les différentes commandes de gestion des paquetages dans un système GNU/Linux Debian 
(apt, dpkg, etc.) munies de leurs principales options. Ce document devra également traiter la gestion des méta-paquets 
- L'installation d'une distribution GNU/Linux Debian sous la forme d'une machine virtuelle gérée par Virtualbox 
- L'écriture du code de l'utilitaire cree_liste_rouge 
- L'écriture du code de la fonction command_not_found_handle 
- L'écriture du code du serveur serecd. On traitera avec soin les contraintes de synchronisation entre les désinstallations 
et installations successives d'un même logiciel en environnement multi-utilisateurs ainsi que les éventuels dysfonctionnements 
produits par les méta-paquets 
- la mesure du gain apporté par l'application SeReC sur la masse logicielle d'un système. Plusieurs scénarios devront être 
définis et testés (ex : système bureautique, système de développement logiciel) 
- la fourniture de SeReC sous la forme d'un paquetage .deb 
- la fourniture d'un manuel d'installation, de configuration et de déploiement de l'application dans lequel chaque opération 
devra être décrite avec précision. 
