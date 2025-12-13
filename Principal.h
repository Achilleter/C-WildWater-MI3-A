#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Structures temporaires

typedef struct{
	char id_usine[64] ; //Colonne 1
	char id_amont[64] ; //Colonne 2
	char id_aval[64] ; //Colonne 3
	float vol ; //Colonne 4 si prblm mettre en double
	float fuites ; //Colonne 5
} Infos;

typedef struct chainon{
    Infos infos;
    struct chainon* premier_enfant;
    struct chainon* dernier_enfant;
    struct chainon* next;
} Noeud;

typedef struct avl_struct
{
    char id_usine[64]; // id de l'usine pour comparaison
    Noeud* val;             // La valeur du nœud
    int eq;                // Facteur d'équilibre (balance factor)
    struct avl_struct *fg; // Pointeur vers le fils gauche
    struct avl_struct *fd; // Pointeur vers le fils droit
} AVLeaks;

//-------------------------------------------