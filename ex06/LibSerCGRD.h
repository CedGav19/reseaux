#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"



void AProposServeurCGRD(char *Version,char *Nom1,char* Nom2) ;

int RechercheCGRD(char* NomFichier,int Reference ,struct VehiculeCGRD
*UnRecord) ;

int ReservationCGRD(char* NomFichier,int Reference ,int Quantite ) ;

int FacturationCGRD(char NomFichier[80], char NomClient[40], time_t Date,int Quantite,int Reference) ;

int VerifDateCGRD(char NomClient[40], int Date) ;
