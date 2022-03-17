# Description generale 

## L'objectif 
L'objectif de ce serveur est d'intercepter une demande du client qui lui même aura été initié par l'appel de la command not found. Le rôle de ce serveur 
est d'installer le programme sur la machine lorsque le client appelle command not found. 

## Ce que ce serveur fait 

- Ce serveur intercepte la demande du client initie par command not found handle 
- Il crée un thread permettant de gérer la demande du client en question 
- il appelle le script d'installation (autrefois appelé par le crontab dans l'ancienne version du projet ) 
- il renvoit le résultat de la commande , 0 si tout s'est bien passé et 1 si il y a eu une défaillance 


# Analyse du Programme 

## Initialisation des différentes structures 

````
    struct sockaddr_in servAddr; 
    memset(&servAddr , 0 , sizeof(servAddr)); 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servAddr.sin_port = htons(ServeurPort);
````

Ici on crée la structure du serveur , cette structure servira à stocker les informations liés 
au serveur. AF_INET signifie que l'on est ouvert aux connexions extérieurs , étant donné que le serveur
sera local , une simple adresse de loopback sur la machine suffira dans notre cas. 

## Lancement de l'ecoute du serveur 


````
    int servSock; 
    if( (servSock = socket(AF_INET , SOCK_STREAM , 0)) < 0){
        perror(" Erreur au niveau de la création du socket fatal error ");
        exit(-1); 	
    }

    if(bind(servSock , (struct sockaddr *) &servAddr , sizeof(servAddr)) < 0){
        perror(" Erreur au niveau de l attache entre le socket et la structure addr ");
        exit(-1); 	
    }
    
    if(listen(servSock , 10) < 0){
        perror(" Erreur au niveau de l ecoute du serveur ");
        exit(-1); 	
    }
   
   ````
   
On commence par créer le socket du serveur. Son but sera de stocker le socket du serveur. 
On attache ensuite la structure réseau du serveur au socket du serveur avec bind et on lance
l'écoute. Au maximum 10 clients pourront se connecter à ce serveur simultanément. Cette quantité 
pourra être ajustée avec le temps. 


## Boucle Principale 

````    for(;;){
    
       struct sockaddr_in clientAddr; 
       socklen_t clientAddrLen = sizeof(clientAddr);
       pthread_t idThread;  

       int clientSock = accept(servSock , (struct sockaddr *) &clientAddr , &clientAddrLen); 
       if(clientSock < 0){
           printf(" Erreur au Niveau de la COnnexion du client Le serveur va se fermer");
	   exit(EXIT_FAILURE); 
       }

       printf(" ON arrive au niveau du pthread_create \n"); 
       pthread_create(&idThread , NULL , communication , &clientSock);
    }
}
````

Dans cette boucle on crée une structure réseau client , on calcule taille de cette structure ainsi qu'une variable thread locale
à la boucle. On accepte ensuite un client demandant de se connecter , et on vérifie si clientSock -1. Si tout s'est bien 
passé jusque là on créer un thread dans lequel on passe l'adresse du socket client ainsi créé. 


## Fonction Communication 

### Premiere Partie 
````
void *communication(void *socketClient){

   int sockClient = *(int*) socketClient; 
   char nomPaquet[TAILLE_MAX_MESSAGE]; 
   char retourNeg[] = "1"; 
   char retourPos[] = "0"; 
   char commande[1000];
   strcpy(commande , "/usr/bin/SeReC/scriptInstallation ");  
   
   int erreur; 
   int retourSysteme = 0; 

   if( (erreur = recv( sockClient , nomPaquet , TAILLE_MAX_MESSAGE , 0) != 0)){
       printf(" erreur au niveau de la reception du serveur pour le thread %d " , (int)pthread_self()); 
   } 
````
Maintenant que l'on a passé l'adresse du socket client en paramère on est en mesure de recevoir 
des messages de ce client. Dans un premier temps on attend que le client envoit le nom du paquet à installer. 

### Seconde Partie 

````
   strcat(commande , nomPaquet); 
   if( (retourSysteme = system(commande)) != 0){
       if( (erreur = send( sockClient , retourNeg , 5 , 0)) < 0 ){
           printf(" Erreur au niveau de l envoi de l erreur   ");
	   exit(EXIT_FAILURE);  
       } 
   } else {
   	if( (send(sockClient , retourPos , 5 , 0)) < 0 ){
	    perror(" Erreur Au niveau de la reponse le serveur va  s arreter"); 
	    exit(EXIT_FAILURE); 
	}
   }
   
   ````
   
   Ici on tente d'installer le paquet client en appelant le script d'installation. Si l'appel system() échoue 
   on envoit 1 au client sous forme d'une chaine de caractère. Dans le cas ou l'appel system renvoit 0 , on renvoit
   0 sous forme de chaine de caractère au client. 
   
   
   







