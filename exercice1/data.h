#ifdef DATAH

#else


#define DATAH
#include <time.h>
// 2022 , interdit de modifier les champs existants

struct VehiculeCGRD { 
         int  Supprime ;
         int  Reference ;
	 char Constructeur[30] ;
   	 char Modele[30] ;
	 int  Quantite ;
	 
      
	} ;

struct FactureCGRD
       {
         int NumeroFacturation ;
         char Acheteur[40] ;
	 time_t DateFacturation ;
         int Quantite;
	 int Reference ;
	} ;  	
#endif
