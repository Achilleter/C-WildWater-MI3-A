#include "principal.h"

AVLeaks* rotationGaucheavleaks(AVLeaks* avl){
    AVLeaks* p = avl->fd;
    int eq_a = avl->eq;
    int eq_p = p->eq;
    avl->fd = p->fg;
    p->fg = avl;
    avl->eq = eq_a - max(eq_p,0) - 1;
    p->eq = min3(eq_a-2, eq_a+eq_p-2, eq_p-1);
    return p;
}

AVLeaks* rotationDroiteavleaks(AVLeaks* avl){
    AVLeaks* p = avl->fg;
    int eq_a = avl->eq;
    int eq_p = p->eq;
    avl->fg = p->fd;
    p->fd = avl;
    avl->eq = eq_a - min(eq_p,0) + 1;
    p->eq = max3(eq_a+2, eq_a+eq_p+2, eq_p+1);
    return p;
}

AVLeaks* doubleRotationGaucheavleaks(AVLeaks* avl){
    avl->fd = rotationDroiteavleaks(avl->fd);
    return rotationGaucheavleaks(avl);
}

AVLeaks* doubleRotationDroiteavleaks(AVLeaks* avl){
    avl->fg = rotationGaucheavleaks(avl->fg);
    return rotationDroiteavleaks(avl);
}

AVLeaks* equilibrerAVLeaks(AVLeaks* avl){
    if(avl->eq >= 2){
        if(avl->fd->eq >= 0)
            return rotationGaucheavleaks(avl);
        else
            return doubleRotationGaucheavleaks(avl);
    }
    else if(avl->eq <= -2){
        if(avl->fg->eq <= 0)
            return rotationDroiteavleaks(avl);
        else
            return doubleRotationDroiteavleaks(avl);
    }
    return avl;
}

float leaks(Noeud* a) {
    if (a == NULL)
        return 0;
    float total = 0;
    while (a != NULL) {
        // fuite du nœud courant
		if(a->infos.fuites > 0 && a->infos.vol > 0){
        	total += a->infos.vol * (a->infos.fuites / 100.0f);
		}
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
		return rechercheAvleaks(avl->fg, id);
	} else {
		// Aller à droite
		return rechercheAvleaks(avl->fd, id);
	}
}

Noeud* incrementleakabr(Noeud* a,AVLeaks* avl,AVLeaks* avltmp, char* id) {
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
	nouveau->id_usine[0] = '\0';
	nouveau->eq = 0 ;
	nouveau->fg = NULL ;
	nouveau->fd = NULL ;
	nouveau->val = malloc(sizeof(Noeud));
	return nouveau;
}

Noeud* creerarbreleaks() {
	Noeud* nouveau = malloc( sizeof( Noeud ) );
	if( nouveau == NULL ) exit( EXIT_FAILURE );
	nouveau->infos.id_usine[0] = '\0';
	nouveau->infos.id_amont[0] = '\0';
	nouveau->infos.id_aval[0] = '\0';
	nouveau->infos.vol= 0;
	nouveau->infos.fuites = 0;
	nouveau->premier_enfant = NULL;
	nouveau->dernier_enfant = NULL;
	nouveau->next = NULL;
	return nouveau;
}

AVLeaks* insertionAVLeaks(AVLeaks* avl, AVLeaks* avltmp, int* h) {
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
		avl->fg = insertionAVLeaks(avl->fg, avltmp,h);
		*h=-*h;
	} else {
		// Aller à droite
		avl->fd = insertionAVLeaks(avl->fd, avltmp,h);
	}

    // Mise à jour du facteur d'équilibre et rééquilibrage si nécessaire
    if (*h != 0)
    {
        avl->eq += *h;
        avl = equilibrerAVLeaks(avl);
        *h = (avl->eq == 0) ? 0 : 1; // Mise à jour de la hauteur
    }
    return avl;
}

Noeud* chercherenfants(AVLeaks** avl, Noeud* a, char id[], FILE* f){
	char c1[64] ,c2[64] ,c3[64] ,c4[64] ,c5[64];
	rewind(f);

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
			AVLeaks* avltmp = creerAVL();
            strncpy( avltmp->val->infos.id_usine , c1, 63 );
		    avltmp->val->infos.id_usine[63] = '\0';
			strcpy(avltmp->id_usine, avltmp->val->infos.id_usine);

		    strncpy( avltmp->val->infos.id_amont, c2, 63 );
		    avltmp->val->infos.id_amont[63] = '\0';

		    strncpy( avltmp->val->infos.id_aval , c3, 63 );
		    avltmp->val->infos.id_aval[63] = '\0';
		
		    avltmp->val->infos.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		    avltmp->val->infos.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
		    int h = 0;
            *avl = insertionAVLeaks(avltmp,*avl,&h);
		a = incrementleakabr(a,*avl,avltmp,id);
		a = chercherenfants(avl,a,avltmp->id_usine,f);
        }
    }
	return a;
}

Noeud* incrementleakavl(AVLeaks* avl, char id[64]) {
    FILE* f = fopen( "c-wildwater_v0.dat" , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", "c-wildwater_v0.dat");	
		exit(1);
	}
	Noeud* a = creerarbreleaks();
	a = chercherenfants(&avl, a, id, f);
    fclose(f);
	return a;
}