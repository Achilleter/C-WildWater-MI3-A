#include "principal.h"

float leaks(Noeud* a) {
    if (a == NULL)
        return 0
    float total = 0;
    while (a != NULL) {
        // fuite du nœud courant
        total += a->infos.vol * (a->infos.fuites / 100.0f);
        // fuite des enfants
        total += leaks(a->premier_enfant);
        // passer au frère suivant
        a = a->next;
    }
    return total;
}

Noeud* rechercheAvleaks(AVLeaks* avl, char* id){ //prends en paramètre l'id de l'amont et retourne celui-ci
    if (avl == NULL) {
		return NULL;
	}

	int cmp = strcmp(id, avl->id_usine);
	if (cmp == 0) {
		// Trouvé le nœud, incrémenter les fuites
		return avl->val;
	} else if (cmp < 0) {
		// Aller à gauche
		rechercheAvleaks(avl->fg, id);
	} else {
		// Aller à droite
		rechercheAvleaks(avl->fd, id);
	}
}

Noeud* incrementleakabr(Noeud* a,AVLeaks avl,AVLeaks avltmp, char* id) {
	if (a == NULL) {
		return NULL;
	}
	Noeud* atmp;
	atmp = rechercheAvleaks(avl, id);
	if(a == NULL) {
		a->premier_enfant = atmp;
		a->dernier_enfant = atmp;
	}
	else{
		a->dernier_enfant->next = atmp;
		a->dernier_enfant = atmp;
	}
	return a;
}

AVLeaks* creerAVL() {
	AVLeaks* nouveau = malloc( sizeof( AVLeaks ) );
	if( nouveau == NULL ) exit( EXIT_FAILURE );
	nouveau->id_usine = "";
	nouveau->eq = 0 ;
	nouveau->fg = NULL ;
	nouveau->fd = NULL ;
	return nouveau;
}

Noeud* creerarbreleaks() {
	Noeud* nouveau = malloc( sizeof( Noeud ) );
	if( nouveau == NULL ) exit( EXIT_FAILURE );
	nouveau->infos = NULL;
	nouveau->premier_enfant = NULL;
	nouveau->dernier_enfant = NULL;
	nouveau->next = NULL;
	return nouveau;
}

AVLeaks* insertionAVLeaks(AVLeaks* avl, AVLeaks avltmp, int* h) {
	if (avl == NULL) {
		*h = 1;
		return avltmp;
	}
	int cmp = strcmp(avltmp->id_usine, avl->id_usine);
    if (cmp == 0) {
		// cette usine existe déjà, retourner la valeur existante
		return avl;
		*h = 0;
	} else if (cmp < 0) {
		// Aller à gauche
		a->fg = insertionAVLeaks(avl->fg, avltmp);
		*h=-*h;
	} else {
		// Aller à droite
		a->fd = insertionAVLeaks(avl->fd, avltmp);
	}

    // Mise à jour du facteur d'équilibre et rééquilibrage si nécessaire
    if (*h != 0)
    {
        a->eq += *h;
        a = equilibrerAVL(a);
        *h = (a->eq == 0) ? 0 : 1; // Mise à jour de la hauteur
    }
    return avl;
}

Noeud* incrementleakavl(AVLeaks* avl, char* id) {
    FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
    AVLeaks* avltmp = creerAVL
	Noeud* a = creerarbreleaks();
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
		while ((ch = fgetc(f)) != '\n' && ch != EOF);

        if (strcmp(id, c2) == 0) {
            strncpy( avltmp->val.infos.id_usine , c1, 63 );
		    avltmp->val.infos.id_usine[63] = '\0';
			avltmp->id_usine = avltmp->val.infos.id_usine;

		    strncpy( avltmp->val.infos.id_amont, c2, 63 );
		    avltmp->val.infos.id_amont[63] = '\0';

		    strncpy( avltmp->val.infos.id_aval , c3, 63 );
		    avltmp->val.infos.id_aval[63] = '\0';
		
		    avltmp->val.infos.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		    avltmp->val.infos.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
            avl = insertavl(avltmp,avl);
			a = incrementleakabr(a,avl,avltmp,id);
        }
    }
    fclose(f);
	return a;
}