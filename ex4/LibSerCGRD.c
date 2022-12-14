#include "LibSerCGRD.h"

void AProposServeurCGRD(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}

int RechercheCGRD(char* NomFichier,int Reference ,struct VehiculeCGRD *UnRecord) 
{
    FILE *sortie ;

    int compteur=0 ;
 sortie = fopen(NomFichier,"r") ; 
    if ( sortie == NULL )
        {
         fprintf(stderr,"Echec Ouverture\n") ;
         exit(0) ;
        }
     else
        fprintf(stderr,"Ouverture reussie \n") ;

  fread(UnRecord,sizeof(struct VehiculeCGRD  ),1,sortie) ;
  while (UnRecord->Reference!=Reference &&  !feof(sortie) ) 
  {
    fread(UnRecord,sizeof(struct VehiculeCGRD  ),1,sortie) ;
    compteur++;
  }
  if(UnRecord->Reference==Reference)
  {
    fclose(sortie);
    return compteur ;
  }
  else 
    { fclose(sortie);
        return -1;} 
}



int ReservationCGRD(char* NomFichier,int Reference ,int Quantite ) 
{
    struct VehiculeCGRD  UnRecord ;
    int offset ;
 if ( (offset=RechercheCGRD(NomFichier , Reference, &UnRecord))!=-1)
      {
        fprintf(stderr," elt  a l'offset %d \n ",offset);
        if (UnRecord.Quantite>=Quantite)
        {
            FILE *fp ;

             fp = fopen(NomFichier,"r+") ; 
            if ( fp == NULL )
                {
                 fprintf(stderr,"Echec Ouverture\n") ;
                 exit(0) ;
                }
                fprintf(stderr,"Ouverture reussie \n") ;

                UnRecord.Quantite =UnRecord.Quantite- Quantite;
                fseek(fp,0,SEEK_SET);
                
                fseek(fp,offset*sizeof(UnRecord),SEEK_CUR);
                fwrite(&UnRecord,sizeof(UnRecord), 1, fp);
                fclose(fp);

                   return 1 ;
        }
        else 
            return 0;
      }
      else 
        return 0;


}
int FacturationCGRD(char NomFichier[80], char NomClient[60], time_t Date,int Quantite,int Reference)
{
    struct FactureCGRD facture;
    FILE *fp;
    int nbFactures = 0;

    //ouverture du fichier en lecture seule :
    fp = fopen(NomFichier,"r");

    //Si il n'y a pas encore d'enregistrements, alors il passera dans le if. De m??me si le fichier s'est mal ouvert.
    if(fp == NULL)
    {
        fprintf(stderr, "Ouverture du fichier impossible : ");
    }
    //calcul du nombre de facture(s) qui se trouvent dans le fichier facture
    else
    {
        fseek(fp,0,SEEK_END);
        nbFactures = (ftell(fp) / sizeof(facture));
        //nbFactures va servir pour les NumeroFacturation.

        fclose(fp);
    }

    //ouverture en mode ajout ?? la fin, si le fichier n'existe pas, alors il sera cr??e

    fp = fopen(NomFichier,"a");

    if(fp == NULL)
    {
        fprintf(stderr, "Ouverture du fichier impossible : ");
        return (-1);
    }
    else
    {
        //copie de toutes les donn??es dans la structure facture
        facture.NumeroFacturation = nbFactures + 1;
        strcpy(facture.Acheteur, NomClient);
        facture.DateFacturation = Date;
        facture.Quantite = Quantite;
        facture.Reference = Reference;

        //??criture dans le fichier
        fwrite(&facture,sizeof(facture), 1, fp);

        //fermeture du fichier
        fclose(fp);

        //Retour du num??ro de la facture, Initialis?? au d??but du else.
        return facture.NumeroFacturation;
    }

    //Si il passe par ici (ce qui ne devrait pas arriver normalement, il retournera -1.
    return -1;

}



