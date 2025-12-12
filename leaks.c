#include "principal.h"

float leaks(Pile* u) {
    if (u == NULL)
        return 0
    float total = 0;
    while (u != NULL) {
        // fuite du nœud courant
        total += u->infos.vol * (u->infos.fuites / 100.0f);
        // fuite des enfants
        total += leaks(u->enfants);
        // passer au frère suivant
        u = u->next;
    }
    return total;
}

void incrementleakavl(Pile* abr, AVLeaks* avl, char* id) {
    FILE* f = fopen( nom , "r" );
	if( f == NULL ) {
		printf("Erreur d'ouverture du fichier %s\n", nom);	
		exit(1);
	}
    AVLeaks* avltmp;
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

		    strncpy( avltmp->val.infos.id_amont, c2, 63 );
		    avltmp->val.infos.id_amont[63] = '\0';

		    strncpy( avltmp->val.infos.id_aval , c3, 63 );
		    avltmp->val.infos.id_aval[63] = '\0';
		
		    avltmp->val.infos.vol = (strcmp( c4 , "-" )==0) ? -1.0 : atof(c4) ;
		    avltmp->val.infos.fuites = (strcmp( c5 , "-" )==0) ? -1.0 : atof(c5) ;
            abr=avltmp->val;
            insertavl(avltmp,avl);
        }
    }
    fclose(f);
}

// void incrementleakabr