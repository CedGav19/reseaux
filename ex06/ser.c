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
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h> 


static void signal_handler(int sig)
{ 
 // siglongjmp(env,sig);
 switch (sig) {
case SIGINT:
 printf("longjumped from interrupt CRTL C %d\n",SIGINT);
 exit(0) ;
 /* On fermerait les fichiers */
 break;
case SIGTSTP:
 printf("longjumped from interrupt CTRL Z %d\n",SIGTSTP);
 printf("Demarrage du sleep \n") ;
 sleep(30) ;
 printf("Fin du sleep \n") ;
 break ;
 } ;
}


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
 int tmp ;

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

 signal(SIGINT, signal_handler);
 signal(SIGTSTP, signal_handler) ;


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

time_t timespand = time(NULL);
int testret ;
switch(UneRequete.Type)
{
   case 1: 
     if ( RechercheCGRD("VehiculesCGRD" ,UneRequete.Reference ,&voiture)!=-1) 
     {
      UneRequete.Reference = voiture.Reference ;
      strcpy(UneRequete.Constructeur,voiture.Constructeur);
      strcpy(UneRequete.Modele,voiture.Modele);
      strcpy(UneRequete.Couleur,voiture.couleur);
      UneRequete.Quantite=voiture.Quantite;
      UneRequete.Type=OK;
       rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteCGRD) ,&sor ) ;
       if ( rc == -1 )
          die("SendDatagram:") ;
       else
         fprintf(stderr,"bytes envoyes:%d\n",rc ) ;



     }
      
   break ;
   case 2: 
     /* printf("------------date = %d\n",UneRequete.Date);
      printf("---------------Acheteur = %s\n",UneRequete.NomClient);*/

         if(VerifDateCGRD(UneRequete.NomClient, UneRequete.Date)==1)
         {
            if(ReservationCGRD("VehiculesCGRD",UneRequete.Reference,UneRequete.Quantite)==1)
            {
               
               if ((tmp=FacturationCGRD("FactureCGRD",UneRequete.NomClient, UneRequete.Date,UneRequete.Reference,UneRequete.Quantite))!=-1)
               {
                  RechercheCGRD("VehiculesCGRD" ,UneRequete.Reference ,&voiture); // remplis les ifnos sur la voiture 
                  strcpy(UneRequete.Constructeur,voiture.Constructeur);
                  strcpy(UneRequete.Modele,voiture.Modele);
                  strcpy(UneRequete.Couleur,voiture.couleur);
                  UneRequete.NumeroFacture = tmp ;
                  UneRequete.Type=OK;
                  SendDatagram(Desc,&UneRequete,sizeof(struct RequeteCGRD) ,&sor );
                  if ( rc == -1 )
                      die("SendDatagram:") ;
                   else
                     fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
               }
            }
         }
         else 
         {
            printf("DOUBLON ! \n");
         }
   break ;
};
 
 

}
 close(Desc) ;
}
