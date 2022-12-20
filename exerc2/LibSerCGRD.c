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
  }
  if(UnRecord->Reference==Reference)
  {
    return 1 ;
  }
  else 
    {return 0 ;}
}



