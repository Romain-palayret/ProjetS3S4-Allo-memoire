#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define VALEUR_RENVOI 5

struct sockaddr_in adserv;
int socketClient; 

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

   printf(" la valeur de retour %s \n" , valeurRetour); 


   retour = atoi(valeurRetour);
   if(retour == 1){
       perror(" LE SERVEUR N A PAS TROUVE LE PAQUET "); 
       exit(EXIT_FAILURE); 
   }
   printf(" le paquet %s a bien ete installe sur le serveur " , nomPaquet);   
}


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
