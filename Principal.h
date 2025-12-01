#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structures temporaires

typedef struct{
	char* id_usine ; //Colonne 1
	char* amont ; //Colonne 2
	char* id_aval ; //Colonne 3
	int amont ; //Colonne 4
	float fuites ; //Colonne 5
} Infos;


typedef struct chainon{   
    Infos infos;
    struct chainon* next;
} Pile;

typedef struct{
    Infos infos;
    Pile* Next;
} Usines;
//-------------------------------------------
Infos* makeInfos(char* usine, char* amont, char* aval , int amont, float fuites ){
	Infos* i = malloc(sizeof( Infos ));




}
