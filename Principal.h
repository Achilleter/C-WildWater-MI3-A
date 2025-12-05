#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structures temporaires

typedef struct{
	char* id_usine ; //Colonne 1
	char* id_amont ; //Colonne 2
	char* id_aval ; //Colonne 3
	float vol ; //Colonne 4 si prblm mettre en double
	float fuites ; //Colonne 5
} Infos;

typedef struct Pile{
    Infos infos;
    Pile* enfants;
    struct Pile* next;
} Pile;

//-------------------------------------------
Infos* makeInfos(char* usine, char* id_amont, char* aval , int amont, float fuites ){
	Infos* i = malloc(sizeof( Infos ));




}