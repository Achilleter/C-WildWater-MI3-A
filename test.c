#include "Principal.h"

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
AVL* creerAVL(const char* id){
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


AVL* rotationGauche(AVL* a){
    AVL* p = a->fd;
    int eq_a = a->u.hauteur;
    int eq_p = p->u.hauteur;
    a->fd = p->fg;
    p->fg = a;
    a->u.hauteur = eq_a - max(eq_p,0) - 1;
    p->u.hauteur = min3(eq_a-2, eq_a+eq_p-2, eq_p-1);
    return p;
}

AVL* rotationDroite(AVL* a){
    AVL* p = a->fg;
    int eq_a = a->u.hauteur;
    int eq_p = p->u.hauteur;
    a->fg = p->fd;
    p->fd = a;
    a->u.hauteur = eq_a - min(eq_p,0) + 1;
    p->u.hauteur = max3(eq_a+2, eq_a+eq_p+2, eq_p+1);
    return p;
}

AVL* doubleRotationGauche(AVL* a){
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

AVL* doubleRotationDroite(AVL* a){
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

AVL* equilibrerAVL(AVL* a){
    if(a->u.hauteur >= 2){
        if(a->fd->u.hauteur >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    }
    else if(a->u.hauteur <= -2){
        if(a->fg->u.hauteur <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }
    return a;
}


AVL* insertionAVL(AVL* a, const char* id, int* h){
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

AVL* rechercherAVL(AVL* a, const char* id){
    if(!a) return NULL;
    int cmp = strcmp(id, a->u.id);
    if(cmp == 0) return a;
    if(cmp < 0) return rechercherAVL(a->fg, id);
    return rechercherAVL(a->fd, id);
}

AVL* get_or_create(AVL** racine, const char* id){
    AVL* u = rechercherAVL(*racine, id);
    if(!u){
        int h = 0;
        *racine = insertionAVL(*racine, id, &h);
        u = rechercherAVL(*racine, id);
    }
    return u;
}


void afficherAVL(AVL* a){
    if(!a) return;
    afficherAVL(a->fg);
    printf("Usine %s | Max %.2f | Src %.2f | Reel %.2f\n",
           a->u.id, a->u.vol_max, a->u.vol_sources, a->u.vol_reel);
    afficherAVL(a->fd);
}
// ---------------------------------------------------------------------------------------------
AVL* histo_max( AVL* Usine , Infos i ){
	int h = 0;
	Usine = insertionAVL( Usine, i.id_amont , &h );
	if(Usine) Usine->u.vol_max += i.vol;
	return Usine;
}

AVL* histo_src( AVL* Usine , Infos i ){
	int h = 0;
	Usine = insertionAVL( Usine, i.id_aval , &h );
	Usine->u.vol_sources += i.vol;
	return Usine;
}

AVL* histo_reel( AVL* Usine , Infos i ){
	int h = 0;
	strcpy(i.id_usine, i.id_aval);
	Usine = insertionAVL(Usine, i.id_aval , &h);
	double reel = i.vol * (1 - i.fuites/100);
	Usine->u.vol_sources += i.vol;
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
	AVL* u = NULL;
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];

	while( 1 ){
		// Initialisation des chaînes de caractères à "-" avant le fscanf
		strcpy(c1,"-");
		strcpy(c2,"-");
		strcpy(c3,"-");
		strcpy(c4,"-");
		strcpy(c5,"-");

		int n = fscanf(f , "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]", c1,c2,c3,c4,c5);

		if (n != 5){
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
		
		if (strcmp(arg1, "histo") == 0) {
			if (verif_S_U(&i) && strcmp(arg2, "max") == 0) {
				u = histo_max(u, i);
			}
			else if (verif_S_U(&i) && strcmp(arg2, "src") == 0) {
				u = histo_src(u, i);
			}
			else if (verif_S_U(&i) && strcmp(arg2, "real") == 0) {
				u = histo_reel(u, i);
			}
		}

		
		
	} 
	afficherAVL(u);
	fclose(f);
}
// ---------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	if (argc < 4) {
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