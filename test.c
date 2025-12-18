#include "Principal.h"

// ---------------------------------------------------------------------------------------------
// Les fonctions suivantes vérifient les différentes lignes du fichier
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

AVL* avoir_ou_création(AVL** racine, const char* id){
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

void libererAVL(AVL* a){
	if(!a) return;
	libererAVL(a->fg);
	libererAVL(a->fd);
	free(a->u.id);
	free(a);
}
// ---------------------------------------------------------------------------------------------
//	HISTO MAX
void histo_maxMAX(AVL* Avl_U, Usine* tab_max) {
	if (Avl_U == NULL) return;

	if (tab_max[0].id == NULL || Avl_U->u.vol_max > tab_max[0].vol_max) {
		tab_max[0] = Avl_U->u;

		// Réordonner (croissant)
		for (int i = 0; i < 9; i++) {
			if (tab_max[i].id != NULL && (tab_max[i + 1].id == NULL || tab_max[i].vol_max > tab_max[i + 1].vol_max)) {
				Usine tmp = tab_max[i];
				tab_max[i] = tab_max[i + 1];
				tab_max[i + 1] = tmp;
			}
		}
	}

	histo_maxMAX(Avl_U->fg, tab_max);
	histo_maxMAX(Avl_U->fd, tab_max);
}

void histo_maxMIN(AVL* Avl_U, Usine* tab_min) {
    if (Avl_U == NULL) return;

    if (tab_min[49].id == NULL || Avl_U->u.vol_max < tab_min[49].vol_max){
    	if(tab_min[48].id == NULL) tab_min[48] = tab_min[49];
        tab_min[49] = Avl_U->u;

        // Réordonner pour que tab_min[0] ait le plus petit vol_max
        for (int i = 48; i >= 0; i--) {
            if (tab_min[i].vol_max > tab_min[i+1].vol_max) {
                Usine tmp = tab_min[i];
                tab_min[i] = tab_min[i+1];
                tab_min[i+1] = tmp;
            } else {
                break; // optimisation
            }
        }
    }

    histo_maxMIN(Avl_U->fg, tab_min);
    histo_maxMIN(Avl_U->fd, tab_min);
}
//	HISTO SRC
void histo_srcMAX(AVL* Avl_U, Usine* tab_max) {
	if (Avl_U == NULL) return;

	if (tab_max[0].id == NULL || Avl_U->u.vol_sources > tab_max[0].vol_sources) {
		tab_max[0] = Avl_U->u;

		// Réordonner (croissant)
		for (int i = 0; i < 9; i++) {
			if (tab_max[i].id != NULL && (tab_max[i + 1].id == NULL || tab_max[i].vol_sources > tab_max[i + 1].vol_sources)) {
				Usine tmp = tab_max[i];
				tab_max[i] = tab_max[i + 1];
				tab_max[i + 1] = tmp;
			}
		}
	}

	histo_srcMAX(Avl_U->fg, tab_max);
	histo_srcMAX(Avl_U->fd, tab_max);
}

void histo_srcMIN(AVL* Avl_U, Usine* tab_min) {
    if (Avl_U == NULL) return;

    if (tab_min[49].id == NULL || Avl_U->u.vol_sources < tab_min[49].vol_sources){
    	if(tab_min[48].id == NULL) tab_min[48] = tab_min[49];
        tab_min[49] = Avl_U->u;

        // Réordonner pour que tab_min[0] ait le plus petit vol_sources
        for (int i = 48; i >= 0; i--) {
            if (tab_min[i].vol_sources > tab_min[i+1].vol_sources) {
                Usine tmp = tab_min[i];
                tab_min[i] = tab_min[i+1];
                tab_min[i+1] = tmp;
            } else {
                break; // optimisation
            }
        }
    }

    histo_srcMIN(Avl_U->fg, tab_min);
    histo_srcMIN(Avl_U->fd, tab_min);
}
//	HISTO REAL
void histo_reelMAX(AVL* Avl_U, Usine* tab_max) {
	if (Avl_U == NULL) return;

	if (tab_max[0].id == NULL || Avl_U->u.vol_reel > tab_max[0].vol_reel) {
		tab_max[0] = Avl_U->u;

		// Réordonner (croissant)
		for (int i = 0; i < 9; i++) {
			if (tab_max[i].id != NULL && (tab_max[i + 1].id == NULL || tab_max[i].vol_reel > tab_max[i + 1].vol_reel)) {
				Usine tmp = tab_max[i];
				tab_max[i] = tab_max[i + 1];
				tab_max[i + 1] = tmp;
			}
		}
	}

	histo_reelMAX(Avl_U->fg, tab_max);
	histo_reelMAX(Avl_U->fd, tab_max);
}

void histo_reelMIN(AVL* Avl_U, Usine* tab_min) {
	if (Avl_U == NULL) return;

	if (tab_min[49].id == NULL || Avl_U->u.vol_reel < tab_min[49].vol_reel){
    	if(tab_min[48].id == NULL) tab_min[48] = tab_min[49];
        tab_min[49] = Avl_U->u;

        // Réordonner pour que tab_min[0] ait le plus petit vol_reel
        for (int i = 48; i >= 0; i--) {
            if (tab_min[i].vol_reel > tab_min[i+1].vol_reel) {
                Usine tmp = tab_min[i];
                tab_min[i] = tab_min[i+1];
                tab_min[i+1] = tmp;
            } else {
                break; // optimisation
            }
        }
    }

    histo_reelMIN(Avl_U->fg, tab_min);
    histo_reelMIN(Avl_U->fd, tab_min);
}
// ---------------------------------------------------------------------------------------------
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
		
		strncpy( i.id_usine , c1, 64 );
		i.id_usine[63] = '\0';

		strncpy( i.id_amont, c2, 64 );
		i.id_amont[63] = '\0';

		strncpy( i.id_aval , c3, 64 );
		i.id_aval[63] = '\0';
		
		i.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		i.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
		
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
				
				
				printf("HISTO MAX \nMax \n");
				for( int i = 0 ; i<10 ; i++ ){
					if ( tab_max[i].id !=NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_max[i].id, tab_max[i].vol_max);
					}
				}
				printf("\n Min: \n");
				for( int i = 0 ; i<50 ; i++ ){
					if ( tab_min[i].id !=NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_min[i].id, tab_min[i].vol_max);
					}
				}
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
				
				
				printf("HISTO SRC \nMax \n");
				for( int i = 0 ; i<10 ; i++ ){
					if ( tab_max[i].id !=NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_max[i].id, tab_max[i].vol_sources);
					}
				}
				printf("\n Min: \n");
				for( int i = 0 ; i<50 ; i++ ){
					if ( tab_min[i].id !=NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_min[i].id, tab_min[i].vol_sources);
					}
				}
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
				
				
				printf("HISTO REAL \nMax \n");
				for( int i = 0 ; i<10 ; i++ ){
					if ( tab_max[i].id !=NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_max[i].id, tab_max[i].vol_reel);
					}
				}
				printf("\n Min: \n");
				for( int i = 0 ; i<50 ; i++ ){
					if ( tab_min[i].id != NULL ){
						printf(" %d, %s , %.2f \n", i+1, tab_min[i].id, tab_min[i].vol_reel);
					}
				}
			}
		}
	fclose(f);
	libererAVL(u);
}
// ---------------------------------------------------------------------------------------------
int main(int argc, char** argv) {
	
	clock_t start = clock();

	if (argc < 4) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		return 0;
	}
	const char *name = argv[1];
	const char *fonction = argv[2];
	const char *details = argv[3];
	
	
	if( strcmp(fonction, "leaks") == 0 ){
		faire_leak(name,details);
	}
	else if ( strcmp(fonction, "histo") == 0 &&
        (strcmp(details, "max") == 0 ||
         strcmp(details, "src") == 0 ||
         strcmp(details, "real") == 0) ){
		incrementationFICHIER(name, fonction , details );
	}
	
	clock_t end = clock();
	double temps = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Temps CPU : %.6f secondes\n", temps);
	
	
	return 0;
}