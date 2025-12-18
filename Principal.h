#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MAX_ID 64

typedef struct{
	char id_usine[MAX_ID] ; //Colonne 1
	char id_amont[MAX_ID] ; //Colonne 2
	char id_aval[MAX_ID] ; //Colonne 3
	float vol ; //Colonne 4 
	float fuites ; //Colonne 5
} Infos;

typedef struct Noeud {
    char id[MAX_ID];
    float fuite;                 // % fuite du tronçon entrant
    struct Noeud *enfants;        // premier enfant
    struct Noeud *next;           // frère suivant
} Noeud;

typedef struct s_pile{
    Infos infos;
    struct s_pile* enfants;
    struct s_pile* next;
} Pile;

typedef struct usine{
	char* id;
	float vol_max;
	float vol_sources;
	float vol_reel;
	int hauteur;
}Usine;

typedef struct pAVLeak {
    char id[MAX_ID];
    Noeud *noeud;
    int eq;
    struct pAVLeak *fg;
    struct pAVLeak *fd;
} AVLeak;

typedef struct u_AVL {
    Usine u;
    struct u_AVL* fg;
    struct u_AVL* fd;
} AVL;
// ------------------------------------------------------------------------------------
//	DEF FONCTION MAIN.C
// ------------------------------------------------------------------------------------
int max(int a, int b);
int min(int a, int b);
int max3(int a, int b, int c);
int min3(int a, int b, int c);
bool verif_S_U( Infos* i );
bool verif_U( Infos* i );
bool verif_U_S( Infos* i );
bool verif_S_J( Infos* i );
AVL* creerAVL(const char* id);
AVL* rotationGauche(AVL* a);
AVL* rotationDroite(AVL* a);
AVL* doubleRotationGauche(AVL* a);
AVL* doubleRotationDroite(AVL* a);
AVL* equilibrerAVL(AVL* a);
AVL* insertionAVL(AVL* a, const char* id, int* h);
AVL* rechercherAVL(AVL* a, const char* id);
void afficherAVL(AVL* a);
void libererAVL(AVL* a);
void histo_maxMAX(AVL* Avl_U, Usine* tab_max);
void histo_maxMIN(AVL* Avl_U, Usine* tab_min);
void histo_srcMAX(AVL* Avl_U, Usine* tab_max);
void histo_srcMIN(AVL* Avl_U, Usine* tab_min);
void histo_reelMAX(AVL* Avl_U, Usine* tab_max);
void histo_reelMIN(AVL* Avl_U, Usine* tab_min);
void incrementationFICHIER( const char* nom , const char* arg1 , const char* arg2);
// ------------------------------------------------------------------------------------
//	DEF FONCTION LEAKS.C
// ------------------------------------------------------------------------------------
int taille(AVLeak *h);
AVLeak *rotation_droite(AVLeak *y);
AVLeak *rotation_gauche(AVLeak *x);
int equilibre(AVLeak *h);
Noeud *creernoeud(const char *id);
AVLeak *rotation(AVLeak *avl, const char *id);
AVLeak *insertavl(AVLeak *avl, const char *id, Noeud **noeud);
Noeud *rechercheavl(AVLeak *avl, const char *id);
void ajouteEnfants(Noeud *parent, Noeud *enfant);
Noeud *verifFichier(const char *fichier, const char *id_usine, AVLeak **index, float *vol_init);
int nbrEnfant(Noeud *n);
float calculerFuites(Noeud *n, float volume);
void freearbre(Noeud *n);
void freeavl(AVLeak *avl);
void faire_leak(const char* nomFICHIER, const char* nomUSINE);
// ------------------------------------------------------------------------------------