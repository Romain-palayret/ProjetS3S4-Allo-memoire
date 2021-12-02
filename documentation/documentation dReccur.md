# Documentation du programme dReccur :

## Description générale
Le programme _dReccur_ est un script bash s'occupant de trier quel paquet n'as pas été utilisé depuis longtemps, puis d'appeller une fonction de desinstallation pour le desinstaller si la désinstallation de celui ci ne présente aucun risque pour la machine Debian.
Ce script sera executer à interval régulier sans l'intervention de l'utilisateur.

## Description de la conception et réalisation du script
On commence par récuperer dans le fichier _serec.config_ le temps en seconde au bout duquel un paquet doit être desinstaller s'il n'est pas utilisé.
Ce temps est le dernier nombre de la première ligne du fichier _serec.config_.
On le stock dans une variable _temps_.

![alt text](./Image/variable_temps_serec.png) 

Dans le fichier listeVerte est stocké les noms des paquets qui ne sont pas nescessaire au fonctionnement de la machine Debian.
Chaque ligne de ce fichier est un nom de paquet.
On parcours donc ce fichier dans une boucle while et, avec la commande find, on stock dans un fichier temporaire _heureAcces_ l'ensemble des dernière heure d'acces de chaque fichier de usr se terminant par le nom du paquet en question.

![alt text](./Image/stockageHeure_serecConfig.png)

![alt text](./Image/contenuHeureAcces_serecConfig.png) voici le contenu du fichier temporaire _heureAcces_ la commande find pour le paquet _gimp_.

Une fois cette liste d'heure fichier récupérée, on essaie de trouver l'heure la plus recente. On est parti du principe que si au moins un de ces fichier à été utilisé recemment, alors le paquet en question ne doit pas être supprimé.
Pour ce faire, on sépare dans des variables l'heure, les minutes, les jours depuis le début de l'année et l'année en cours. Puis on convertit tout en seconde.

![alt text](./Image/extractionHeureLigne_serecConfig.png)

Dans une boucle while qui parcours le fichier temporaire, on recherche l'heure la plus recente.

![alt text](./Image/boucleWhile_serecConfig.png)

Une fois qu'on a trouvé l'heure la plus recente, on la compare avec l'heure courante (elle aussi convertit en seconde) auquel on soustrait le temps contenu dans le fichier _serec.config_.
Si le paquet n'as pas été utilisé depuis longtemps, on appelle la fonction de desinstallation.

![image](./Image/comparaisonHeure_serecConfig.png)