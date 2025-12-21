#include "Principal.h"

/* ============================================================================================
   FONCTIONS HISTOGRAMME
   Ces fonctions parcourent l'AVL pour récupérer les plus grands et plus petits volumes
   MAX / SRC / REAL
   tab_max et tab_min servent à stocker les n premières usines max et min
   ============================================================================================ */

//	HISTO MAX
//parcours récursif pour récupérer les volumes max de vol_max
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

//parcours récursif pour récupérer les volumes min de vol_max
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
//parcours récursif pour récupérer les volumes max de vol_sources
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

//parcours récursif pour récupérer les volumes min de vol_sources
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
//parcours récursif pour récupérer les volumes max de vol_reel
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

//parcours récursif pour récupérer les volumes min de vol_reel
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
/* ============================================================================================
   ÉCRITURE DU FICHIER HISTOGRAMME
   tab_max et tab_min contiennent les usines max et min respectivement
   type = 0 => vol_max, 1 => vol_sources, 2 => vol_reel
   ============================================================================================ */
void ecrire_fichier_histo( Usine *tab_max, int nmax,Usine *tab_min, int nmin,int type){
    FILE *f = fopen("histo.dat", "w");
	if (!f) {
        printf("Erreur : impossible de créer histo.dat\n");
        return;
    }
    // Écriture des valeurs maximales
    for (int i = 0; i < nmax; i++) {
        if (tab_max[i].id != NULL) {
            double v;
            if (type == 0) {
                v = tab_max[i].vol_max;
            }
            else if (type == 1) {
                v = tab_max[i].vol_sources;
            }
            else {
                v = tab_max[i].vol_reel;
            }
            fprintf(f, "%s,%.2f,max\n", tab_max[i].id, v);
        }
    }
    // Écriture des valeurs minimales
    for (int i = 0; i < nmin; i++) {
        if (tab_min[i].id != NULL) {
            double v;
            if (type == 0) {
                v = tab_min[i].vol_max;
            }
            else if (type == 1) {
                v = tab_min[i].vol_sources;
            }
            else {
                v = tab_min[i].vol_reel;
            }
            fprintf(f, "%s,%.2f,min\n", tab_min[i].id, v);
        }
    }
    fclose(f);
}