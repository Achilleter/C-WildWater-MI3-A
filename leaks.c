#include "Principal.h"

int taille(AVLeak *h) {
    return h ? h->eq : 0;
}

AVLeak *rotation_droite(AVLeak *y) {
    AVLeak *x = y->fg;
    AVLeak *T2 = x->fd;

    x->fd = y;
    y->fg = T2;

    y->eq = max(taille(y->fg), taille(y->fd)) + 1;
    x->eq = max(taille(x->fg), taille(x->fd)) + 1;

    return x;
}

AVLeak *rotation_gauche(AVLeak *x) {
    AVLeak *y = x->fd;
    AVLeak *T2 = y->fg;

    y->fg = x;
    x->fd = T2;

    x->eq = max(taille(x->fg), taille(x->fd)) + 1;
    y->eq = max(taille(y->fg), taille(y->fd)) + 1;

    return y;
}

int equilibre(AVLeak *h) {
    return h ? taille(h->fg) - taille(h->fd) : 0;
}

Noeud *creernoeud(const char *id) {
    Noeud *n = malloc(sizeof(Noeud));
    if (!n) {
        printf("Erreur allocation mémoire \n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->id, id, MAX_ID - 1);
    n->id[MAX_ID - 1] = '\0';
    n->fuite = 0;
    n->enfants = NULL;
    n->next = NULL;
    return n;
}

AVLeak *rotation(AVLeak *avl, const char *id){
    avl->eq = 1 + max(taille(avl->fg), taille(avl->fd));
    int h = equilibre(avl);
    // Rotation droite
    if (h > 1 && strcmp(id, avl->fg->id) < 0)
        return rotation_droite(avl);
    
    // Rotation gauche
    if (h < -1 && strcmp(id, avl->fd->id) > 0)
        return rotation_gauche(avl);
    
    // Double rotation gauche-droite
    if (h > 1 && strcmp(id, avl->fg->id) > 0) {
        avl->fg = rotation_gauche(avl->fg);
        return rotation_droite(avl);
    }
    
    // Double rotation droite-gauche
    if (h < -1 && strcmp(id, avl->fd->id) < 0) {
        avl->fd = rotation_droite(avl->fd);
        return rotation_gauche(avl);
    }
    return avl;
}

AVLeak *insertavl(AVLeak *avl, const char *id, Noeud **noeud) {
    if (!avl) {
        AVLeak *avltmp = malloc(sizeof(AVLeak));
        if (!avltmp) {
            printf("Erreur allocation mémoire \n");
            exit(EXIT_FAILURE);
        }
        avltmp->noeud = creernoeud(id);
        strncpy(avltmp->id, id, MAX_ID - 1);
        avltmp->id[MAX_ID - 1] = '\0';
        avltmp->fg = NULL;
        avltmp->fd = NULL;
        avltmp->eq = 1;
        *noeud = avltmp->noeud;
        return avltmp;
    }

    if (strcmp(id, avl->id) < 0)
        avl->fg = insertavl(avl->fg, id, noeud);
    else if (strcmp(id, avl->id) > 0)
        avl->fd = insertavl(avl->fd, id, noeud);
    else {
        *noeud = avl->noeud;
        return avl;
    }

    avl=rotation(avl,id);

    return avl;
}

Noeud *rechercheavl(AVLeak *avl, const char *id) {
    if (!avl) {
        return NULL;
    }
    int cmp = strcmp(id, avl->id);
    if (cmp == 0) {
        return avl->noeud;
    }
    if (cmp < 0) {
        return rechercheavl(avl->fg, id);
    }
    return rechercheavl(avl->fd, id);
}

void ajouteEnfants(Noeud *parent, Noeud *enfant) {
    enfant->next = parent->enfants;
    parent->enfants = enfant;
}

Noeud *verifFichier(const char *fichier, const char *id_usine, AVLeak **index, float *vol_init)
{
    FILE *f = fopen(fichier, "r");
    if (!f) return NULL;

    char c1[64], c2[64], c3[64], c4[64], c5[64];
    Noeud *racine = NULL;

    while (fscanf(f, "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n",
                  c1, c2, c3, c4, c5) == 5)
    {
        Infos i = {"", "", "", -1, -1};
        strcpy(i.id_usine, c1);
        strcpy(i.id_amont, c2);
        strcpy(i.id_aval,  c3);
        i.vol    = strcmp(c4, "-") ? atof(c4) : -1;
        i.fuites = strcmp(c5, "-") ? atof(c5) : -1;

        // SOURCE → USINE
        if (verif_S_U(&i) && strcmp(c3, id_usine) == 0) *vol_init = i.vol;

        Noeud *amont = NULL, *aval = NULL;

        if (strcmp(c2, "-") != 0) *index = insertavl(*index, c2, &amont);
        if (strcmp(c3, "-") != 0) *index = insertavl(*index, c3, &aval);

        if (amont && aval) {
            ajouteEnfants(amont, aval);
            if (i.fuites >= 0)
                aval->fuite = i.fuites;
        }

        if (!racine && strcmp(c3, id_usine) == 0)
            racine = aval;
        if (!racine && strcmp(c2, id_usine) == 0)
            racine = amont;
    }

    fclose(f);
    return racine;
}


int nbrEnfant(Noeud *n) {
    int i = 0;
    Noeud *e = n->enfants;
    while (e != NULL) {
        i++;
        e = e->next;
    }
    return i;
}

float calculerFuites(Noeud *n, float volume) {
    if (!n || volume <= 0) return 0;

    // Calculer le volume perdu par fuite
    float perdu = volume * n->fuite / 100.0f;
    float restant = volume - perdu;

    int nb = nbrEnfant(n);
    float total = perdu;

    // Si pas d'enfants, retourner juste les fuites de ce nœud
    if (nb == 0) return total;

    // Distribuer équitablement le volume restant aux enfants
    float part = restant / nb;
    for (Noeud *e = n->enfants; e; e = e->next)
        total += calculerFuites(e, part);

    return total;
}

void freearbre(Noeud *n) {
    if (!n) return;
    for (Noeud *e = n->enfants; e; ) {
        Noeud *tmp = e;
        e = e->next;
        freearbre(tmp);
    }
    free(n);
}

void freeavl(AVLeak *avl) {
    if (!avl) return;
    freeavl(avl->fg);
    freeavl(avl->fd);
    free(avl);
}


void faire_leak(const char* nomFICHIER, const char* nomUSINE){
	AVLeak *index = NULL;
	float volume_initial = 0;
	Noeud *racine = verifFichier(nomFICHIER, nomUSINE, &index, &volume_initial);

	if (!racine) {
		printf("fuites = -1\n");
		freeavl(index);
		return exit(EXIT_FAILURE);
	}

	float pertes = calculerFuites(racine, volume_initial);

	printf("Pertes totales pour l'usine %s : %.2f m³/an\n", nomUSINE, pertes);

	freearbre(racine);
	freeavl(index);
}