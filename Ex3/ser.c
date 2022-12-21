 /*--------------------------------------
  Herman Vanstapel
  
  ex02\ser.c 
  
  Un serveur recevant une structure 
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "./udplib/udplib.h"
#include "requeteCGRD.h"
#include "data.h"
#include "LibSerCGRD.h"

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

int main(int argc,char *argv[])
{
    int rc ;
    int Desc ;
    struct sockaddr_in sthis ; /* this ce programme */
    struct sockaddr_in sos ; /* s = serveur */
    struct sockaddr_in sor ; /* r = remote */
    
    u_long  IpSocket ;
    u_short PortSocket ;
    
    int tm ;

   /*  requete en requeteCGRD*/
    struct RequeteCGRD UneRequete ;
    struct VehiculeCGRD voiture ;

    memset(&sthis,0,sizeof(struct sockaddr_in)) ;
    memset(&sos,0,sizeof(struct sockaddr_in)) ; 
    memset(&sor,0,sizeof(struct sockaddr_in)) ; 
    
    printf("Ceci est le serveur\n") ;
    if ( argc!=3)
    {
     printf("ser ser port cli\n") ;
     exit(1) ;
    }
    
    /* Récupération IP & port   */
    IpSocket= inet_addr(argv[1]);
    PortSocket = atoi(argv[2]);
    // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
    Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
    
    if ( Desc == -1 )
       die("CreateSockets:") ;
    else
       fprintf(stderr,"CreateSockets %d\n",Desc) ;
    
   while(1)
   {
    /* struct requete en requeteCGRD*/
    tm = sizeof(struct RequeteCGRD) ;
    rc = ReceiveDatagram( Desc,&UneRequete ,tm, &sor ) ;
    if ( rc == -1 )
       die("ReceiveDatagram") ;
    /*
    else
      fprintf(stderr,"bytes recus:%d:%s\n",rc,UneRequete.Message ) ;
    */
    printf("Type recu %d\n", UneRequete.Type) ;

    /* attention l'enum peut être codé en short */
    /* reponse avec psos */
    /*
    UneRequete.Type = Reponse ; 
    strcat(UneRequete.Message," Client") ;
    */
    //on effectue la requète de recherche après l'avoir reçue
    /* struct requete en requeteCGRD*/

      switch(UneRequete.Type)
      {
         case 1: 
            RechercheCGRD("VehiculesCGRD" ,UneRequete.Reference ,&voiture) ;
            UneRequete.Reference = voiture.Reference ;
            strcpy(UneRequete.Constructeur,voiture.Constructeur);
            strcpy(UneRequete.Modele,voiture.Modele);
            strcpy(UneRequete.Couleur,voiture.couleur);
         break ;
      };
       


       //on renvoie le résultat de la requète
       rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteCGRD) ,&sor ) ;
       if ( rc == -1 )
          die("SendDatagram:") ;
       else
         fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
       
      
   }
   close(Desc) ;
}