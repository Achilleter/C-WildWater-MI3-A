#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structures temporaires

typedef struct{
    char* id_amont;
    char* id_usine;
    char* id_aval;
    float vol;
    float fuites;
} Infos;


typedef struct chainon{   
    Infos infos;
    struct chainon* next;
} Pile;

typedef struct{
    Infos infos;
    Pile* Next;
} Usines;