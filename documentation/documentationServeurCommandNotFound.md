#Description generale 

## L'objectif 
L'objectif de ce serveur est d'intercepter une demande du client qui lui même aura été initié par l'appel de la command not found. Le rôle de ce serveur 
est d'installer le programme sur la machine lorsque le client appelle command not found. 

## Ce que ce serveur fait 

- Ce serveur intercepte la demande du client initie par command not found handle 
- Il crée un thread permettant de gérer la demande du client en question 
- il appelle le script d'installation (autrefois appelé par le crontab dans l'ancienne version du projet ) 
- il renvoit le résultat de la commande , 0 si tout s'est bien passé et 1 si il y a eu une défaillance 


#Analyse du Programme 

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
