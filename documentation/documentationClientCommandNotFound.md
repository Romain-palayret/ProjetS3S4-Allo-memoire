# Description Générale 

Ce client sera déclenché après l'appel à command not found handle, il va contacter le serveur local lancé 
depuis l'utilisateur Serec et lui demander d'installer le paquet qui n'a pas été trouvé. Si ce paquet 
peut être installé , le serveur renverra 0 , sinon 1. Ce client gère la l'envoi de la requête et la reception 
de la demande. 


# Analyse du programme. 

## Analyse de la structure du programme 

````
int main(int argc , char **argv){

    socketClient = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); 
    if(socketClient < 0){
        perror(" Erreur création du socket ");
	      exit(EXIT_FAILURE); 
    }

    adserv.sin_family=AF_INET; 
    adserv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    adserv.sin_port = htons(50000); 
    
    if(connect(socketClient , (struct sockaddr *) &adserv , sizeof(adserv)) < 0){
        perror(" Connexion refusee ");
        exit(EXIT_FAILURE); 	
    }
    communication(argv[1]);
}
````

Le main se contente de créer le socket client et de remplir la structure serveur , puis de se connecter au serveur local
il fait ensuite appel à la fonction communication pour prendre le relais en passant le nom du paquet qui n'a pas été trouvé. 

## Analyse de la fonction Communication 


````
int communication(char nomPaquet[]){
    
   char valeurRetour[10];
   int retour;  
   if( (send(socketClient , nomPaquet , strlen(nomPaquet) , 0)) < 0){
       perror(" Erreur au niveau de l envoi du nom du paquet "); 
       exit(EXIT_FAILURE);  
   } 
   
   if( (recv(socketClient , valeurRetour , 10 , 0)) < 0 ){
      perror(" erreur au niveau du code de retour "); 
      exit(EXIT_FAILURE);  
   }

   retour = atoi(valeurRetour);
   if(retour == 1){
       perror(" LE SERVEUR N A PAS TROUVE LE PAQUET "); 
       exit(EXIT_FAILURE); 
   }
}
````

La fonction envoit le nom du paquet , elle attend ensuite une réponse avec recv. Elle converti ensuite la valeur de retour
avec atoi. Si le retour est 1 on renvoit un message d'erreur , sinon on considère que tout s'est bien passé. 






