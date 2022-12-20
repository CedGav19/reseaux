/*---------------------------------------------------------------
   Vanstapel Herman
   ex02\cli.c

 Le client dit bonjour en utilisant un structure  et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "./udplib/udplib.h"
#include "requeteCGRD.h"

/*
#include <stdio.h>
#include <string.h>
#include "./udplib/udplib.h"
#include "structure.h"
*/
void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
 int rc ;
 int Desc ;
 int tm ; 
 
 u_long  IpSocket , IpServer;
 u_short PortSocket, PortServer ; 
 
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 /*changement de requete en  RequeteCGRD */
 struct RequeteCGRD UneRequete ;

 memset(&sthis,0,sizeof(struct sockaddr_in)) ;
 memset(&sos,0,sizeof(struct sockaddr_in)) ; 
 memset(&sor,0,sizeof(struct sockaddr_in)) ; 
 
 if (argc!=5)
  
{
    printf("cli client portc serveur ports\n") ;
    exit(1) ;
 }
 
 /* Récupération IP & port   */
 IpSocket= inet_addr(argv[1]);
 PortSocket = atoi(argv[2]);
 
 IpServer = inet_addr(argv[3]) ;
 PortServer = atoi(argv[4]);
 
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ; 
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 
  sos.sin_family = AF_INET ;
  sos.sin_addr.s_addr= IpServer ;
  sos.sin_port = htons(PortServer) ;

 UneRequete.Type = Question ; 

//affiche de la requiète avant envoie
printf("avant l'envoie de la requete ! \n ");
AfficheRequeteCGRD(stdout, UneRequete );
//on assigne 2 pour que la recherche soit effectué sur cette référence
UneRequete.Reference = 2 ;


 /*
 strncpy(Requete.Message , "Avec une structure: Bonjour" , sizeof(Requete.Message)) ;
*/ 
/* struct requete en requeteCGRD*/
 rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteCGRD) ,&sos ) ;

 if ( rc == -1 )
    die("SendDatagram") ;
 else
   fprintf(stderr,"Envoi de %d bytes\n",rc ) ;
 
 /* struct requete en requeteCGRD*/
 memset(&UneRequete,0,sizeof(struct RequeteCGRD)) ;
 tm = sizeof(struct RequeteCGRD) ;
 

 //on reçois les données de la recherche
  rc = ReceiveDatagram( Desc, &UneRequete,tm, &sor ) ;
 if ( rc == -1 )
    die("ReceiveDatagram") ;

 else
   fprintf(stderr,"bytes recus:%d:%d\n",rc,UneRequete.Type ) ;
 
printf("apres la reception de la requete ! \n ");
   AfficheRequeteCGRD(stdout, UneRequete );

 close(Desc) ;
}
