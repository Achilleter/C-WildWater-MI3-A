#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char id_usine[64] ; //Colonne 1
	char id_amont[64] ; //Colonne 2
	char id_aval[64] ; //Colonne 3
	float vol ; //Colonne 4 si prblm mettre en double
	float fuites ; //Colonne 5
} Infos;

void incrementationFICHIER( const char* nom ){
	FILE* f = fopen( nom , "r" );
	if( f == NULL ) exit(1);
	
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];
	Infos* i = malloc(sizeof(Infos));
	if( i == NULL ) exit(1);
	
	while( fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n", c1,c2,c3,c4,c5) == 5 ){
		
		strncpy( i->id_usine , c1, 63 );
		i->id_usine[63] = '\0';
		strncpy( i->id_amont, c2, 63 );
		i->id_amont[63] = '\0';
		strncpy( i->id_aval , c3, 63 );
		i->id_aval[63] = '\0';
		
		i->vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i->fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
		
		printf(" %s | %s | %s | %f | %f \n", i->id_usine, i->id_amont, i->id_aval, i->vol, i->fuites); 
	}
	

	fclose(f);	
	free(i);
}


int main() {
    incrementationFICHIER("c-wildwater_v0(1).dat");
    return 0;
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char id_usine[64] ; //Colonne 1
	char id_amont[64] ; //Colonne 2
	char id_aval[64] ; //Colonne 3
	float vol ; //Colonne 4 si prblm mettre en double
	float fuites ; //Colonne 5
} Infos;

FILE* incrementationFICHIER( const char* nom ){
	FILE* f = fopen( nom , "r" );
	if( f == NULL ) exit(1);
	
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];
	Infos* i = malloc(sizeof(Infos));
	if( i == NULL ) exit(1);
	
	while( fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n", c1,c2,c3,c4,c5) == 5 ){
		
		strcpy( i->id_usine , c1 );
        i->id_usine[63] = '\0';
		strcpy( i->id_amont, c2 );
        i->id_amont[63] = '\0';
		strcpy( i->id_aval , c3 );
        i->id_aval[63] = '\0';
		
		i->vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i->fuites = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c5) ;
		
		printf(" %s | %s | %s | %f | %f \n", i->id_usine, i->id_amont, i->id_aval, i->vol, i->fuites); 
	}
	

	fclose(f);	
    free(i);
	return f;
}


int main() {
    incrementationFICHIER("c-wildwater_v0(1).dat");
    return 0;
}

*/