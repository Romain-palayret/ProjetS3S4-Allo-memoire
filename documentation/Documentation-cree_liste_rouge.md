# Documentation du programme de la création de la liste rouge :

Le programme _createdRedList_ est un script bash qui permet de créer une liste de paquet contenant la priorité "important" et la priorité "required".  
- Les paquets de priorités "important" sont paquets devant obligatoirement être présents sur tous systèmes de type Unix.
- Les paquets de priorités "required" sont nécessaire au bon fonctionnement du système. La suppression d'un paquet avec cette priorité peut entrainer une panne et un dysfonctionnement partiel ou total du sytème. Parmis les risques possibles et fréquents, la désinstalation du paquet "dpkg" sinifie qu'il est impossible d'installer de nouveau paquet et ainsi ne pas pouvoir faire évoluer votre système. 