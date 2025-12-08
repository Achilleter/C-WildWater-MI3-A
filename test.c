#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
	char id_usine[64] ; //Colonne 1
	char id_amont[64] ; //Colonne 2
	char id_aval[64] ; //Colonne 3
	float vol ; //Colonne 4 
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
//-----------------------------------------------------------------------------------
void incrementationFICHIER( const char* nom ){
	FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
	
	Infos i;
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];

	while( 1 ){
		// Initialisation des chaînes de caractères à "-" avant le fscanf
		strcpy(c1,"-");
		strcpy(c2,"-");
		strcpy(c3,"-");
		strcpy(c4,"-");
		strcpy(c5,"-");

		int n = fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]", c1,c2,c3,c4,c5);

		if (n == EOF){
			break;
		}
		
		int ch;
		while ((ch = fgetc(f)) != '\n' && ch != EOF); // Nous permet de prendre la ligne au complet & fgetc nous permet de lire le caractère suivant sur la ligne
		
		strncpy( i.id_usine , c1, 63 );
		i.id_usine[63] = '\0';

		strncpy( i.id_amont, c2, 63 );
		i.id_amont[63] = '\0';

		strncpy( i.id_aval , c3, 63 );
		i.id_aval[63] = '\0';
		
		i.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;

		if( verif_U(&i) ){
			printf("%s | %s | %s | ",i.id_usine, i.id_amont, i.id_aval);
			
			if (i.vol == -1.0 ) printf("- | ");
			else printf("%.2f | ", i.vol);

			if (i.fuites == -1.0 ) printf("-\n");
			else printf("%.2f\n", i.fuites);
		} 
	} 
	fclose(f);
}
//-----------------------------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	const char *name = argv[1];
	
	incrementationFICHIER(name);
	return 0;
}

//-----------------------------------------------------------------------------------
/*
typedef struct Pile{
    Infos infos;
    struct Pile* enfants;
    struct Pile* suivant;
} Pile;

Pile makePile( Infos info ){
    Pile p;
    p.infos = info;
    p.enfants = NULL;
    p.suivant = NULL;
    return p;
}


Pile* ajoutPile( Pile* tete, Infos info ){
    Pile* nouveau = (Pile*)malloc( sizeof(Pile) );
    if( nouveau == NULL ){
        printf("Erreur d'allocation mémoire\n");
        exit(1);
    }
    *nouveau = makePile( info );
    nouveau->suivant = tete;
    return nouveau;
}
//-----------------------------------------------------------------------------------
Pile makeAVL( Pile* tete , Infos i ){
    if( tete == NULL && verif_S_U( &i ) ){
        Pile p = makePile( i );
        return p;
    }
    else if( verif_S_U(&tete->infos) && verif_U( &i ) && strcmp( tete->infos.id_aval , i.id_amont ) == 0 ){
        Pile p = makePile( i );
        tete->enfants = ajoutPile( tete->enfants , i );
        return p;
    }

    else if( verif_U( &tete->enfants->infos ) && verif_U_S( &i ) && strcmp( tete->infos.id_amont, i.id_amont ) == 0 ){
        Pile p = makePile( i );
        tete->enfants->enfants = ajoutPile( tete->enfants->enfants , i );
        return p;
    }

    else if( verif_U_S( &tete->enfants->infos ) && verif_S_J( &i ) && strcmp( tete->infos.id_amont, i.id_usine ) == 0 ){
        Pile p = makePile( i );
        tete->enfants->enfants = ajoutPile( tete->enfants->enfants , i );
        return p;
    }
    
    printf("Erreur de structure ou de liaison entre les éléments\n");
    exit(1);
}
*/
//-----------------------------------------------------------------------------------
