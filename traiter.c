#include"Principal.h"

// Fonction qui va nous perettre d'extraire les données du ficher
FILE* incrémentationFICHIER( const char* nom ){
	FILE* f = fopen( nom , "r" );
	if( f == NULL ) exit(ERROR_FICHIER);
	
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];
	Infos i;
	
	while( fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n", c1,c2,c3,c4,c5) == 5 ){
		
		strcpy( i.id_usine , c1 );
		strcpy( i.id_amont, c2 );
		strcpy( i.id_aval , c3 );
		
		i.amont = (srtcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i.fuites = (srtcmp( c4 , "-" )==0) ? -1.0 : atof(c5) ;
		
		//printf(" %s | %s | %s | %f | %f \n", i.id_usine, i.id_amont, i.id_aval, i.amont, i.fuites); 
	}
	
	fclose(f);	

}

// Les fonctions suivantes vérifient les différentes lignes du fichier
// Cette fonction vérifie : SOURCE -> USINE
bool verif_S-U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont!=NULL && i->fuites!=NULL
}
// Cette fonction vérifie : USINE
bool verif_U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval==NULL && i->amont!=NULL && i->fuites==NULL
}
// Cette fonction vérifie : USINE->STOCKAGE
bool verif_U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont==NULL && i->fuites!=NULL
}
// Cette fonction vérifie : STOCKAGE->JONCTION
bool verif_U( Infos* i ){
	return i->id_usine!=NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont==NULL && i->fuites!=NULL
}
