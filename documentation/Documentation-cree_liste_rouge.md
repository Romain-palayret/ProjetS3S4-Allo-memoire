# Documentation du programme de la création de la liste rouge :

## Description générale
Le programme _createdRedList_ est un script bash qui permet de créer une liste de paquet contenant la priorité "important" et la priorité "required".  
- Les paquets de priorités "important" sont paquets devant obligatoirement être présents sur tous systèmes de type Unix.
- Les paquets de priorités "required" sont nécessaire au bon fonctionnement du système. La suppression d'un paquet avec cette priorité peut entrainer une panne et un dysfonctionnement partiel ou total du sytème. Parmis les risques possibles et fréquents, la désinstalation du paquet "dpkg" sinifie qu'il est impossible d'installer de nouveau paquet et ainsi ne pas pouvoir faire évoluer votre système. 

## Description de la conception et réalisation du script
Au début du script, on crée un fichier _file_ ainsi q'un fichier _listeRouge_. Par la suite, on redirige la sortie de commande _dpkg --status_ dans le fichier _file_. Après cela, on lit chaque ligne du fichier _file_. Lorsqu'une ligne contenant la chaîne "Package: " est trouvée on initialise une variable avec la valeurs 3 car il faut aller chercher la ligne ou l'on trouve la priortité du paquet touvé.
![alt text](C:\Users\coren\OneDrive\Images\Captures d’écran\Capture_dpkg--status.png) 