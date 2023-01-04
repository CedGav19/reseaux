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
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h> 

void signal_handler(int sig)
{

}

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
struct sigaction act;

      //Armement du signal
      act.sa_handler = signal_handler;
      act.sa_flags = 0 ;
      sigemptyset (&act.sa_mask);

       sigaction (SIGALRM, &act, NULL);



 int rc ;
 int Desc ;
 int tm ;
 int Compteur=0; 
 int nbrequete=0;
 char tampon[80];
 char c ;
 
 u_long  IpSocket , IpServer;
 u_short PortSocket, PortServer ; 
 
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 /*changement de requete en  RequeteCGRD */
 struct RequeteCGRD UneRequete,reception ;

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
/*tout ce qui est au dessus est de la 
mise en place , on lie nos cli et serevur ensemble.
et on les complete grace aux arguments donne dans la console*/




 
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ; 
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 
  sos.sin_family = AF_INET ;
  sos.sin_addr.s_addr= IpServer ;
  sos.sin_port = htons(PortServer) ;// donne par le prof aussi 




do{

   do {
    printf("------------------\n") ;
    printf("1) Demander une reference\n") ;
    printf("2) Acheter une voiture\n") ;
    printf("3) Quitter\n") ;
     printf("------------------\n") ;
     fflush(stdin );
     scanf ("%d",&UneRequete.Type ); 
   }while (UneRequete.Type >3 && UneRequete.Type <1);

   switch(UneRequete.Type)
   {
      case 1 : 
               do {
               printf(" reference: \n") ;
                scanf ("%d",&UneRequete.Reference ); 
               }while ( UneRequete.Reference<0);

               /* strncpy(Requete.Message , "Avec une structure: Bonjour" , sizeof(Requete.Message)) ;
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
                 rc = ReceiveDatagram( Desc, &reception,tm, &sor ) ;
                if ( rc == -1 )
                   die("ReceiveDatagram") ;

                else
                  fprintf(stderr, "bytes recus:%d:%d\n", rc, reception.Reference);

                  if (reception.Type == OK)
                     fprintf(stdout,"Reference : %d, Constructeur : %s, Modele : %s, Couleur : %s , Quantite : %d !\n", reception.Reference,reception.Constructeur,reception.Modele,reception.Couleur,reception.Quantite);
                  else
                     fprintf(stderr, "Aucun vehicule trouve !\n");


      break ; 
      case 2 : 
               do {

                  c=getchar();// vider le buffer 

                  
                   printf("nom client : \n") ; 
                   fflush(stdin);
                   fgets(tampon   , sizeof(tampon), stdin); 
                   strcpy(UneRequete.NomClient,tampon);

               printf(" reference: \n") ;
                scanf ("%d",&UneRequete.Reference );
                 printf(" QUANTITÉ: \n") ;
                scanf ("%d",&UneRequete.Quantite );
               }while ( UneRequete.Reference<0);

                     printf("\nAffichage de la requete AVANT redo : \n");
                     AfficheRequeteCGRD(stderr,UneRequete);
                     printf("\n");



                     redo: // point pour un jump 

                     //On fixe un délai de 9 secondes pour le timeout et l’appel de la fonction alarm provoquera ce timeout dans neuf secondes.->3 fcature car 30/9=3,...
                     
                   alarm(9); // alarm permettra un appel fonction => arrete la fonction bloquantemais renvoie -1 d'ou le goto redo 


                     rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteCGRD) ,&sos ) ;
                     

                     if(rc == -1)
                     {
                        die("SendDatagram");
                     }
                     else
                     {
                        fprintf(stderr,"Envoi de %d bytes  ");
                        nbrequete++;
                     }


                     do
                     {

                        memset(&reception,0,sizeof(struct RequeteCGRD)) ;
                       
                        //réception du numéro de facture envoyé par le serveur.
                        rc = ReceiveDatagram( Desc, &reception,sizeof(struct RequeteCGRD) , &sor ) ;

                        /*Je me mets en attente de la réception de la réponse à mon message envoyé.
                        Si je ne reçoit pas de réponse, Un déclenchement du timout provoquera un arret de
                        l’appel système Receidatagram avec le code d’erreur -1 , message Interrupted system call.
                        Dans ce cas on fait un branchement à redo*/
                        if ( rc<=0)
                        {
                           perror("error sur receive:") ;
                           fprintf(stderr,"rc %d \n",rc) ;
                           goto redo ;
                        }

                        nbrequete--;
                        /*Si le timer n’expire, j’ai alors reçu une réponse mais il faut voir que la réponse envoyée par le serveur
                        correspond au message envoyé via le numéro. Si ce n’est pas le cas on à affaire à un doublon ,
                        message en double reçut suite à des timeouts avec le précédent message*/
                        alarm(0);
                        if ( nbrequete !=0 )
                        {
                           printf(" ATTENTION doublon !!!!!\n") ;
                           
                        }
                        // J’ai reçu la bonne réponse, je sort de la boucle pour passer au message suivant
                        else
                          {
                           if(reception.Type == OK)
                           {
                              fprintf(stderr,"Nombre de bytes recus : %d bytes \n", rc);

                              fprintf(stdout, "--------[ Facture cree avec succes ]--------\n");
                              fprintf(stdout, "--------[ Achat reussi : Facture N° %d ]--------\n",reception.NumeroFacture);
                              fprintf(stdout, "Client: %s   Constructeur: %s   Modele : %s  , Quantite: %d    \n",reception.NomClient,reception.Constructeur ,reception.Modele,reception.Quantite);
                           }
                           else
                           {
                              fprintf(stderr, "Il y a eu un soucis avec la demande d'achat de vehicule.\n");
                           }


                              Compteur++ ;
                              sleep(5) ;
                          }
                     }while(nbrequete !=0);


               break;
      case 3 : exit(1);
      break ;


   }


    
   }while(1);
    //close(Desc) ; plus necessaire on a un exit quand on chosit 3
}
