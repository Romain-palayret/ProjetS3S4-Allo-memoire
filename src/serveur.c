#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> 
#define TAILLE_MAX_MESSAGE 500
#define  ServeurPort  50000 


typedef struct Client{
    int sockfd; 
    char nom[28]; 

}Client; 



void *communication(void *socketClient){

   int sockClient = *(int*) socketClient; 
   char nomPaquet[TAILLE_MAX_MESSAGE]; 
   char retourNeg[] = "1"; 
   char retourPos[] = "0"; 
   char commande[1000];
   strcpy(commande , "/usr/bin/SeReC/scriptInstallation ");  
   
   int erreur; 
   int retourSysteme = 0; 

   printf(" NOuveau Client \n "); 

   if( (erreur = recv( sockClient , nomPaquet , TAILLE_MAX_MESSAGE , 0) != 0)){
       printf(" erreur au niveau de la reception du serveur pour le thread %d " , (int)pthread_self()); 
   
   } 
   
   printf(" MESSAGE ENVOYE PAR LE CLIENT : %s \n" , nomPaquet); 
  
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
}


int main(int argc , char **argv){


    struct sockaddr_in servAddr; 
    memset(&servAddr , 0 , sizeof(servAddr)); 
    servAddr.sin_family = AF_INET; 
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servAddr.sin_port = htons(ServeurPort);



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


    printf(" ON ARRIVE ICI \n"); 

    for(;;){
    
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
