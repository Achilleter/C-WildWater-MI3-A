#include "Principal.h"

/* ============================================================================================
   FONCTIONS UTILITAIRES MATHÉMATIQUES
   Ces fonctions servent principalement au calcul des hauteurs et équilibres de l’AVL
   ============================================================================================ */

// Retourne le maximum entre deux entiers */
int max(int a, int b) {
    return a > b ? a : b;
}

// Retourne le minimum entre deux entiers 
int min(int a, int b) {
    return a < b ? a : b;
}

// Retourne le maximum entre trois entiers 
int max3(int a, int b, int c) {
    return max(max(a, b), c);
}

// Retourne le minimum entre trois entiers 
int min3(int a, int b, int c) {
    return min(min(a, b), c);
}

/* ============================================================================================
   FONCTIONS DE VALIDATION DES LIGNES DU FICHIER
   Elles permettent d’identifier le type de tronçon décrit par une ligne
   ============================================================================================ */
// Cette fonction vérifie : SOURCE -> USINE
bool verif_S_U(Infos* i) {
    if (!i) return false;
    return
        strcmp(i->id_usine, "-") == 0 &&
        strcmp(i->id_amont, "-") != 0 &&
        strcmp(i->id_aval, "-") != 0 &&
        i->vol != -1.0 &&
        i->fuites != -1.0;
}

// Cette fonction vérifie : USINE
bool verif_U( Infos* i ){
	if (!i) return false;
	return strcmp(i->id_usine, "-")==0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")==0 && 
	i->vol!=-1.0 && 
	i->fuites==-1.0;
}
// Cette fonction vérifie : USINE->STOCKAGE
bool verif_U_S( Infos* i ){
    if (!i) return false;
	return strcmp(i->id_usine, "-")==0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")!=0 && 
	i->vol==-1.0 && 
	i->fuites!=-1.0;
}
// Cette fonction vérifie : STOCKAGE->JONCTION
bool verif_S_J( Infos* i ){
    if (!i) return false;
	return strcmp(i->id_usine, "-")!=0 && 
	strcmp(i->id_amont, "-")!=0 && 
	strcmp(i->id_aval, "-")!=0 && 
	i->vol==-1.0 && 
	i->fuites!=-1.0;
}
/* ============================================================================================
   AVL : CRÉATION ET ROTATION
   Chaque nœud représente une usine et stocke ses volumes
   ============================================================================================ */
// Crée un nouveau nœud AVL
AVL* creerAVL(const char* id){
	if (!id || id[0] == '\0') return NULL;
    AVL* n = malloc(sizeof(AVL));
    if(!n) exit(EXIT_FAILURE);
    n->u.id = malloc(strlen(id)+1);
    strcpy(n->u.id, id);
    n->u.vol_max = 0;
    n->u.vol_sources = 0;
    n->u.vol_reel = 0;
    n->u.hauteur = 0;
    n->fg = n->fd = NULL;
    return n;
}

//Rotation simple gauche 
AVL* rotationGauche(AVL* a){
	if (!a || !a->fd) return a;
    AVL* p = a->fd;
    int eq_a = a->u.hauteur;
    int eq_p = p->u.hauteur;
    a->fd = p->fg;
    p->fg = a;
    a->u.hauteur = eq_a - max(eq_p,0) - 1;
    p->u.hauteur = min3(eq_a-2, eq_a+eq_p-2, eq_p-1);
    return p;
}

//Rotation simple droite
AVL* rotationDroite(AVL* a){
	if (!a || !a->fg) return a;
    AVL* p = a->fg;
    int eq_a = a->u.hauteur;
    int eq_p = p->u.hauteur;
    a->fg = p->fd;
    p->fd = a;
    a->u.hauteur = eq_a - min(eq_p,0) + 1;
    p->u.hauteur = max3(eq_a+2, eq_a+eq_p+2, eq_p+1);
    return p;
}

//Double rotation gauche
AVL* doubleRotationGauche(AVL* a){
	if (!a) return a;
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

//Double rotation droite
AVL* doubleRotationDroite(AVL* a){
	if (!a) return a;
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

//Rééquilibre un nœud AVL
AVL* equilibrerAVL(AVL* a){
	if (!a) return a;
    if(a->u.hauteur >= 2 && a->fd){
        if(a->fd->u.hauteur >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    }
    else if(a->u.hauteur <= -2 && a->fg){
        if(a->fg->u.hauteur <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }
    return a;
}

//Insertion d’un identifiant dans l’AVL
AVL* insertionAVL(AVL* a, const char* id, int* h){
	if (!id || !h) return a;
    if(!a){
        *h = 1;
        return creerAVL(id);
    }

    int cmp = strcmp(id, a->u.id);
    if(cmp < 0){
        a->fg = insertionAVL(a->fg, id, h);
        *h = -*h;
    }
    else if(cmp > 0){
        a->fd = insertionAVL(a->fd, id, h);
    }
    else{
        *h = 0;
        return a; // déjà existante
    }

    if(*h){
        a->u.hauteur += *h;
        a = equilibrerAVL(a);
        *h = (a->u.hauteur == 0) ? 0 : 1;
    }
    return a;
}

//Recherche d’un identifiant dans l’AVL
AVL* rechercherAVL(AVL* a, const char* id){
	if (!a || !id) return NULL;
    int cmp = strcmp(id, a->u.id);
    if(cmp == 0) return a;
    if(cmp < 0) return rechercherAVL(a->fg, id);
    return rechercherAVL(a->fd, id);
}

//Récupère une usine existante ou la crée si absente
AVL* avoir_ou_création(AVL** racine, const char* id){
	if (!racine || !id) return NULL;
    AVL* u = rechercherAVL(*racine, id);
    if(!u){
        int h = 0;
        *racine = insertionAVL(*racine, id, &h);
        u = rechercherAVL(*racine, id);
    }
    return u;
}

//Affichage de l’AVL
void afficherAVL(AVL* a){
    if(!a) return;
    afficherAVL(a->fg);
    printf("Usine %s | Max %.2f | Src %.2f | Reel %.2f\n",
           a->u.id, a->u.vol_max, a->u.vol_sources, a->u.vol_reel);
    afficherAVL(a->fd);
}

//Libération complète de l’AVL
void libererAVL(AVL* a){
	if(!a) return;
	libererAVL(a->fg);
	libererAVL(a->fd);
	free(a->u.id);
	free(a);
}

/* ============================================================================================
   INCRÉMENTATION DU FICHIER DE DONNÉES
   Lecture du fichier d’usines et création de l’AVL
   Calcul des volumes et écriture du fichier histo.dat
   ============================================================================================ */
void incrementationFICHIER( const char* nom , const char* arg1 , const char* arg2){

	FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
	AVL* u = NULL ;
	Infos i;
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];

	while( 1 ){
		// Initialisation des chaînes de caractères à "-" avant le fscanf
		strncpy(c1,"-", 64);
		strncpy(c2,"-", 64);
		strncpy(c3,"-", 64);
		strncpy(c4,"-", 64);
		strncpy(c5,"-", 64);
		c1[63] = '\0'; c2[63] = '\0'; c3[63] = '\0'; c4[63] = '\0'; c5[63] = '\0';

		int n = fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]", c1,c2,c3,c4,c5);

		if (n != 5){
			break;
		}
		
		int ch;
		while ((ch = fgetc(f)) != '\n' && ch != EOF); // Nous permet de prendre la ligne au complet & fgetc nous permet de lire le caractère suivant sur la ligne
		
        // Stockage dans la structure Infos
		strncpy( i.id_usine , c1, 64 );
		i.id_usine[63] = '\0';

		strncpy( i.id_amont, c2, 64 );
		i.id_amont[63] = '\0';

		strncpy( i.id_aval , c3, 64 );
		i.id_aval[63] = '\0';
		
		i.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
		
        // Traitement des lignes selon le type
		if(verif_U(&i)){
		    AVL * v = avoir_ou_création(&u, i.id_amont);
		    v->u.vol_max += i.vol;
		}
		else if(verif_S_U(&i)){
		    AVL * v  = avoir_ou_création(&u, i.id_aval);
		    v->u.vol_sources += i.vol;
		    v->u.vol_reel += i.vol * (1 - i.fuites/100);
		}	
	}
    // Selon le type demandé, remplir les histogrammes
	if (strcmp(arg1, "histo") == 0) {
			if (strcmp(arg2, "max") == 0) {
				Usine tab_max[10];
				for (int i = 0; i < 10; i++){
					tab_max[i].vol_max = -1;
					tab_max[i].id = NULL;
				}

				histo_maxMAX(u, tab_max);

				Usine tab_min[50];
				tab_min[49]= tab_max[9];
				for (int i = 0; i < 49; i++){
					tab_min[i].vol_max= 1e9;
					tab_min[i].id = NULL;
				}

				histo_maxMIN(u, tab_min);
				
				ecrire_fichier_histo(tab_max,10,tab_min,50,0);
			}
			else if (strcmp(arg2, "src") == 0) {
				Usine tab_max[10];
				for (int i = 0; i < 10; i++){
					tab_max[i].vol_sources = -1;
					tab_max[i].id = NULL;
				}

				histo_srcMAX(u, tab_max);

				Usine tab_min[50];
				tab_min[49]= tab_max[9];
				for (int i = 0; i < 49; i++){
					tab_min[i].vol_sources= 1e9;
					tab_min[i].id = NULL;
				}

				histo_srcMIN(u, tab_min);
				
				
				ecrire_fichier_histo(tab_max,10,tab_min,50,1);
			}
			else if (strcmp(arg2, "real") == 0) {
				Usine tab_max[10];
				for (int i = 0; i < 10; i++){
					tab_max[i].vol_reel = -1; 
					tab_max[i].id = NULL;
				}

				histo_reelMAX(u, tab_max);

				Usine tab_min[50];
				tab_min[49]= tab_max[9];
				for (int i = 0; i < 49; i++){
					tab_min[i].vol_reel= 1e9;
					tab_min[i].id = NULL;
				}

				histo_reelMIN(u, tab_min);
				
				
				ecrire_fichier_histo(tab_max,10,tab_min,50,2);
			}
		}
	fclose(f);
	libererAVL(u);
}
