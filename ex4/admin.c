/******************************************
  Herman Vanstapel
  2017 Basé sur Fichiers
*******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "LibSerCGRD.h"

// include pour les fonctions entrees sortie
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

static struct termios old, new;
 
//clrscr() function definition
// #include <stdlib.h>
void ClrScr(void)
{
    system("clear");
}

void Gotoxy(int x,int y)    
{
    printf("%c[%d;%df",0x1B,y,x);
}

/* Initialize new terminal i/o settings */
// #include <stdio.h>
// #include <stdlib.h>

void initTermios(int echo) 
{
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}
 
/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}
 
/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
/* 
Read 1 character without echo 
getch() function definition.
*/
char Getch(void) 
{
  return getch_(0);
}
 
/* 
Read 1 character with echo 
getche() function definition.
*/
char GetchE(void) 
{
  return getch_(1);
}



// permet de supprimer le cr qui placé dans la chaine lors d'un fgets
void DelNewLine(char *Chaine)
{

 Chaine[strlen(Chaine)-1] = 0 ;
}



void MonPrintf(char* tempo, int espace,int taille )
{
 int Count ;
 printf("%s",tempo) ; 
 Count = espace-taille ;
 while ( Count>0 )
 {
       printf(" ");
       Count-- ;
 }
}

void AfficheEnteteVehiculeCGRD ()
{
 char Tampon[80] ;
 sprintf(Tampon,"%s","Ref" ) ;MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Constructeur") ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Modele") ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s","Quantite") ; MonPrintf(Tampon,6,strlen(Tampon)) ;
sprintf(Tampon,"%s","Coueleur") ; MonPrintf(Tampon,20,strlen(Tampon)) ;
 printf("\n") ;
}

void AfficheVehiculeCGRD (struct VehiculeCGRD    *UnRecord)
{
 char Tampon[80] ;
 sprintf(Tampon,"%d",UnRecord->Reference ) ;  MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UnRecord->Constructeur ) ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UnRecord->Modele ) ;    MonPrintf(Tampon,30,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UnRecord->Quantite ) ; MonPrintf(Tampon,6,strlen(Tampon)) ;
  sprintf(Tampon,"%s",UnRecord->couleur ) ; MonPrintf(Tampon,20,strlen(Tampon)) ;
 printf("\n") ;
}

void SaiSieVehiculeCGRD (int Reference, struct VehiculeCGRD  *UnRecord )
{
 char Tampon[80] ;

 printf("Reference :%d \n",Reference) ;
 UnRecord->Reference = Reference ; 
 printf("Saisie Constructeur :") ;
 fgets(UnRecord->Constructeur ,sizeof UnRecord->Constructeur ,stdin ) ;
 printf("Saisie Modele :") ;
 fgets(UnRecord->Modele ,sizeof UnRecord->Modele ,stdin ) ;
 printf("Saisie Quantite :") ;
 fgets(Tampon,sizeof Tampon,stdin ) ;
 UnRecord -> Quantite = atoi(Tampon) ;
 printf("Saisie Couleur :") ;
 fgets(UnRecord->couleur ,sizeof UnRecord->couleur ,stdin ) ;
 
 DelNewLine(UnRecord->Constructeur) ;
 DelNewLine(UnRecord->Modele) ;
 AfficheEnteteVehiculeCGRD () ;
 AfficheVehiculeCGRD (UnRecord) ;
 printf("-----------------------\n") ;
 return ;
}

int NombreVehiculesCGRD (char *NomFichier)
{
 FILE *sortie ;
 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Pas de fichier\n") ;
     return 0 ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;
 fseek(sortie, 0, SEEK_END ) ;
 return (ftell(sortie)/ sizeof(struct VehiculeCGRD )) ;
}

void CreationAjoutVehiculeCGRD (char *NomFichier,struct VehiculeCGRD  *UnRecord )
{
 FILE *sortie ;
 char Redo ;
 int nbr ;

 sortie = fopen(NomFichier,"a") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 
  printf("Position actuelle dans le fichier %ld\n",ftell(sortie)) ;
  UnRecord-> Supprime = 0 ;
  nbr = fwrite(UnRecord,sizeof(struct VehiculeCGRD  ),1,sortie) ;
  fflush(sortie) ;
 fclose(sortie) ;
}



void AfficheFacture(struct FactureCGRD *UneFacture)
{
 char Tampon[80] ;
 sprintf(Tampon,"%d",UneFacture->NumeroFacturation ) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%s",UneFacture->Acheteur ) ; MonPrintf(Tampon,60,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UneFacture->Quantite  ) ; MonPrintf(Tampon,4,strlen(Tampon)) ;
 sprintf(Tampon,"%d",UneFacture->Reference) ; MonPrintf(Tampon,4,strlen(Tampon)) ;

 time_t rawtime ;
 rawtime= UneFacture->DateFacturation;
 struct tm *info ;
 info = gmtime(&rawtime);

 printf (" %2d: %2d\n", (info->tm_hour +1)%24, info->tm_min);
 
 printf("\n") ;
}

void ListingVehiculesCGRD (char *NomFichier)
{
 struct VehiculeCGRD   UnRecord ;
 FILE *sortie ;
 char Tampon[80] ;
 int  Numero ;
 int  nbr ;

 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 AfficheEnteteVehiculeCGRD () ;
 nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 
 while ( !feof(sortie) )
 {
  fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
  AfficheVehiculeCGRD ( &UnRecord) ;
  nbr = fread(&UnRecord,sizeof(UnRecord),1,sortie) ;
 }
 fclose(sortie) ;
}

void ListingFacturesCGRD(char *NomFichier)
{
 struct FactureCGRD UneFacture ;
 FILE *sortie ;
 char Tampon[80] ;
 int  Numero ;
 int  nbr ;
 

 sortie = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( sortie == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;

 
 nbr = fread(&UneFacture,sizeof(struct FactureCGRD),1,sortie) ;
 
 while ( !feof(sortie) )
 {
  fprintf(stderr,"Record lu %d et Position actuelle dans le fichier %ld\n",nbr,ftell(sortie)) ;
  AfficheFacture( &UneFacture) ;
  nbr = fread(&UneFacture,sizeof(struct FactureCGRD ),1,sortie) ;
 }
 fclose(sortie) ;
}

int main()
{
AProposServeurCGRD("V1" ,"GavageCedric","RomainDemonty");
 char Choix ;
 char Tampon[80] ;
 char nomclient[20];
 int res ;
 struct VehiculeCGRD  UnRecord ;
 int Numero ;
 int cmb;

 int ref;
 while(1)
 {
  printf("-------2022-----------\n") ;
  printf("1) Ajout              \n") ;
  printf("2) Vehicule           \n") ; 
  printf("4) Recherche          \n") ;   
  printf("5) Achat              \n") ;  
  printf("6) Factures           \n") ;
  printf("7) A propos           \n") ;
  printf("8) exit               \n") ; 
  printf("----------------------\n") ; 
  printf(">>") ; Choix=GetchE() ; printf("\n") ;
  switch(Choix)
  {
   case '1': 
             {
              struct VehiculeCGRD   UnRecord ;
 	     FILE *sortie ;
             char Redo ;
		
             Redo='y' ;
 	    while ( Redo=='Y' || Redo=='y')
 	    { 
             int Nombre ;
             Nombre= NombreVehiculesCGRD ("VehiculesCGRD") ;
  	     SaiSieVehiculeCGRD (Nombre+1, &UnRecord ) ;
  	     CreationAjoutVehiculeCGRD ("VehiculesCGRD",&UnRecord) ; 
  	     printf("Encoder un autre (Y/N) ?)") ;
  	     printf(">>") ; Redo=GetchE() ; printf("\n") ;
 	   }
 
	     break ;	
             }
   case '2': ListingVehiculesCGRD ("VehiculesCGRD") ;
             break ;
   case '4': 
  
       printf("reference : ") ; scanf("%d",&ref);
      if ( RechercheCGRD("VehiculesCGRD" , ref, &UnRecord)!=-1)
      {
        AfficheEnteteVehiculeCGRD () ;
        AfficheVehiculeCGRD(&UnRecord);
      }

             break ;
    case '5': 
        printf("nom client : \n") ; 
         fgets(nomclient   , sizeof(nomclient), stdin); 
        printf("reference : \n") ; scanf("%d",&ref);
        printf("quantité : \n") ; scanf("%d",&cmb);

        printf(" on execute l'achat de %d %d fois",ref,cmb);
       if (ReservationCGRD("VehiculesCGRD" , ref, cmb)==1)
       {
            time_t timespand = time(NULL);
           if( FacturationCGRD("FactureCGRD", nomclient, timespand,cmb,ref) !=-1)
            printf("facturation reussie\n");

       }

         break;
   case '6': ListingFacturesCGRD("FactureCGRD") ;
	     break ;
   case '7': AProposServeurCGRD("V 1","CedricRomain","GavageDemonty") ;
             break ;
   
	
   case '8': exit(0) ; 
  }
 }
}
