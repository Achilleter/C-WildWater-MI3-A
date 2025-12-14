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

typedef struct UsineAVL {
    Usine u;
    struct UsineAVL* fg;
    struct UsineAVL* fd;
} UsineAVL;

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

int max3(int a, int b, int c) {
    return max(max(a, b), c);
}
int min3(int a, int b, int c) {
    return min(min(a, b), c);
}


// ---------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------
UsineAVL* creerAVL( char* x ){

	UsineAVL* nouveau = malloc( sizeof( UsineAVL ) );
	if( nouveau == NULL ) exit( EXIT_FAILURE );
	
	nouveau->u.id = malloc( strlen(x) + 1);
	if (!nouveau->u.id) exit(EXIT_FAILURE);
		strcpy(nouveau->u.id,x);
		
	nouveau->u.vol_max = 0 ;
	nouveau->u.vol_sources = 0;
	nouveau->u.vol_reel = 0 ;
	nouveau->u.hauteur = 0 ;
	
	nouveau->fg = NULL ;
	nouveau->fd = NULL ;

	return nouveau;

}
UsineAVL* rotationGauche(UsineAVL* a)
{
	UsineAVL* pivot = a->fd; // Le fils droit devient le pivot
	int eq_a = a->u.hauteur, eq_p = pivot->u.hauteur;

	a->fd = pivot->fg; // Le sous-arbre gauche du pivot devient le fils droit de `a`
	pivot->fg = a;     // `a` devient le fils gauche du pivot

	// Mise à jour des facteurs d'équilibre
	a->u.hauteur = eq_a - max(eq_p, 0) - 1;
	pivot->u.hauteur = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

	return pivot; // Le pivot devient la nouvelle racine
}
UsineAVL* rotationDroite(UsineAVL* a)
{
	UsineAVL* pivot = a->fg; // Le fils gauche devient le pivot
	int eq_a = a->u.hauteur, eq_p = pivot->u.hauteur;

	a->fg = pivot->fd; // Le sous-arbre droit du pivot devient le fils gauche de `a`
	pivot->fd = a;     // `a` devient le fils droit du pivot

	// Mise à jour des facteurs d'équilibre
	a->u.hauteur = eq_a - min(eq_p, 0) + 1;
	pivot->u.hauteur = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

	return pivot; // Le pivot devient la nouvelle racine
}
UsineAVL* doubleRotationGauche(UsineAVL* a)
{
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}
UsineAVL* doubleRotationDroite(UsineAVL* a)
{
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}
UsineAVL* equilibrerAVL(UsineAVL* a)
{
	if (a->u.hauteur >= 2)
	{ // Cas où l'arbre est déséquilibré à droite
		if (a->fd->u.hauteur >= 0)
		{
			return rotationGauche(a); // Rotation simple gauche
		}
		else
		{
			return doubleRotationGauche(a); // Double rotation gauche
		}
	}
	else if (a->u.hauteur <= -2)
	{ // Cas où l'arbre est déséquilibré à gauche
		if (a->fg->u.hauteur <= 0)
		{
			return rotationDroite(a); // Rotation simple droite
		}
		else
		{
			return doubleRotationDroite(a); // Double rotation droite
		}
	}
	return a; // Aucun rééquilibrage nécessaire
}
UsineAVL* insertionAVL(UsineAVL* a, Infos e, int *h)
{
	if (a == NULL){           // Si l'arbre est vide, crée un nouveau nœud
		*h = 1; // La hauteur a augmenté
		return creerAVL(e.id_usine);
	}
	int cmp = strcmp(e.id_usine, a->u.id);
	if (cmp < 0) {
		a->fg = insertionAVL(a->fg, e, h);
		*h = -*h;
	}
	else if (cmp > 0) {
		a->fd = insertionAVL(a->fd, e, h);
	}
	else {
		*h = 0;
	return a;
	}

	// Mise à jour du facteur d'équilibre et rééquilibrage si nécessaire
	if (*h != 0){
		a->u.hauteur += *h;
		a = equilibrerAVL(a);
		*h = (a->u.hauteur == 0) ? 0 : 1; // Mise à jour de la hauteur
	}
	return a;
}
UsineAVL* suppMinAVL(UsineAVL* a, int *h, int *pe)
{
	UsineAVL* temp;
	if (a->fg == NULL)
	{                   // Trouvé le plus petit élément
		*h = -1;        // Réduction de la hauteur
		temp = a;
		a = a->fd; // Le sous-arbre droit devient la racine
		free(temp->u.id);
		free(temp);
		return a;
	}
	else
	{
		a->fg = suppMinAVL(a->fg, h, pe); // Recherche récursive à gauche
		*h = -*h;
	}

	// Mise à jour et rééquilibrage après suppression
	if (*h != 0)
	{
		a->u.hauteur += *h;
		a = equilibrerAVL(a);
		*h = (a->u.hauteur == 0) ? -1 : 0;
	}
	return a;
}
void traiter( UsineAVL* a){
	if( a ){
		printf(" id : %s \n Vol_max : %f \n Vol_src : %f \n Vol_reel : %f \n ", a->u.id, a->u.vol_max, a->u.vol_sources, a->u.vol_reel);
	}
}
void parcoursPostfixe(UsineAVL* a) {
    if (a!=NULL) {
        if (a->fg){
            parcoursPostfixe(a->fg); // Parcours du fils gauche
        }
        if (a->fd) {
            parcoursPostfixe(a->fd); // Parcours du fils droit
        }
    }
    traiter(a);                 // Affiche la valeur du nœud après ses fils
}
// ---------------------------------------------------------------------------------------------
UsineAVL* histo_max( UsineAVL* Usine , Infos i ){
	Usine = creerAVL( i.id_amont );
	if(Usine) Usine->u.vol_max += i.vol;
	return Usine;
}

UsineAVL* histo_src( UsineAVL* Usine , Infos i ){
	Usine = creerAVL( i.id_aval );
	Usine->u.vol_sources += i.vol;
	return Usine;
}

UsineAVL* histo_reel( UsineAVL* Usine , Infos i ){
	Usine = creerAVL( i.id_aval );
	Usine->u.vol_sources += i.vol;
	double reel = i.vol * (1 - i.fuites/100);
	Usine->u.vol_reel += reel;
	return Usine;
}
// ---------------------------------------------------------------------------------------------
void incrementationFICHIER( const char* nom , const char* arg1 , const char* arg2){

	FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
	
	Infos i;
	UsineAVL* u = NULL;
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
		
		if( verif_S_U(&i) ){
			int h = 0;
			u = insertionAVL( u , i , &h );
		}
		
		
	} 
	parcoursPostfixe(u);
	fclose(f);
}
/*
if( verif_U(&i) && strcmp(arg1, "histo") == 0 && strcmp(arg2, "max") == 0 ){
			UsineAVL* u = NULL;
			u = histo_max( u , i );
		}
		
		if( verif_S_U(&i) ){
			if( strcmp(arg1, "histo") == 0 && strcmp(arg2, "src") == 0 ){
				UsineAVL* u = NULL;
				u = histo_src( u , i );
			}
			else if( strcmp(arg1, "histo") == 0 && strcmp(arg2, "reel") != 0){
				UsineAVL* u = NULL;
				u = histo_reel( u , i );
			}
		}
*/
// ---------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		return 0;
	}
	const char *name = argv[1];
	const char *fonction = argv[2];
	const char *details = argv[3];
	
	
	if( strcmp(fonction, "histo") != 0 && strcmp(fonction, "leaks") != 0 ){
		printf("Erreur dans le 2ème argument\n");
		return 0;
	}
	
	else if ( strcmp(fonction, "histo") == 0 &&
        (strcmp(details, "max") != 0 &&
         strcmp(details, "src") != 0 &&
         strcmp(details, "real") != 0) ){
		printf("Erreur dans le 3ème argument\n");
		return 0;
	}
	
	
	incrementationFICHIER(name, fonction , details );
	return 0;
}