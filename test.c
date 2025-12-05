#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	char id_usine[64] ; //Colonne 1
	char id_amont[64] ; //Colonne 2
	char id_aval[64] ; //Colonne 3
	float vol ; //Colonne 4 si prblm mettre en double
	float fuites ; //Colonne 5
} Infos;

// Les fonctions suivantes vérifient les différentes lignes du fichier
// Cette fonction vérifie : SOURCE -> USINE
bool verif_S_U( Infos* i ){
	return strcmp(i->id_usine, "-") == 0 && 
	strcmp(i->id_amont, "-") != 0 && 
	strcmp(i->id_aval, "-") != 0 && 
	i->vol != -1.0 && 
	i->fuites != -1.0;
}
// Cette fonction vérifie : USINE
bool verif_U( Infos* i ){
	return strcmp(i->id_usine, "-")==0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")==0 && 
	i->vol!=-1.0 && 
	i->fuites==-1.0;
}
// Cette fonction vérifie : USINE->STOCKAGE
bool verif_U_S( Infos* i ){
	return strcmp(i->id_usine, "-")==0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")!=0 && 
	i->vol==-1.0 && 
	i->fuites!=-1.0;
}
// Cette fonction vérifie : STOCKAGE->JONCTION
bool verif_S_J( Infos* i ){
	return strcmp(i->id_usine, "-")!=0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")!=0 && 
	i->vol==-1.0 && 
	i->fuites!=-1.0;
}

void incrementationFICHIER( const char* nom ){
	FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];
	Infos i = {0};
	
	while( fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]", c1,c2,c3,c4,c5) > 0 ){
		
		strncpy( i.id_usine , c1, 63 );
		i.id_usine[63] = '\0';
		strncpy( i.id_amont, c2, 63 );
		i.id_amont[63] = '\0';
		strncpy( i.id_aval , c3, 63 );
		i.id_aval[63] = '\0';
		
		i.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
		if( verif_U(&i) ){
			printf(" %s | %s | %s | %.2f | %.2f \n", i.id_usine, i.id_amont, i.id_aval, i.vol, i.fuites); 
		}
	}
	

	fclose(f);
}


int main() {
    incrementationFICHIER("c-wildwater_v0 (1).dat");
    return 0;
}