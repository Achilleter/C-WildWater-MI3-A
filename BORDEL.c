#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 64
typedef struct Noeud {
    char id[MAX_ID];
    float fuite;                 // % fuite du tronçon entrant
    struct Noeud *enfants;        // premier enfant
    struct Noeud *next;           // frère suivant
} Noeud;

typedef struct AVL {
    char id[MAX_ID];
    Noeud *noeud;
    int eq;
    struct AVL *fg;
    struct AVL *fd;
} AVL;

int taille(AVL *h) {
    return h ? h->eq : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVL *rotation_droite(AVL *y) {
    AVL *x = y->fg;
    AVL *T2 = x->fd;

    x->fd = y;
    y->fg = T2;

    y->eq = max(taille(y->fg), taille(y->fd)) + 1;
    x->eq = max(taille(x->fg), taille(x->fd)) + 1;

    return x;
}

AVL *rotation_gauche(AVL *x) {
    AVL *y = x->fd;
    AVL *T2 = y->fg;

    y->fg = x;
    x->fd = T2;

    x->eq = max(taille(x->fg), taille(x->fd)) + 1;
    y->eq = max(taille(y->fg), taille(y->fd)) + 1;

    return y;
}

int equilibre(AVL *h) {
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

AVL *rotation(AVL *avl, const char *id){
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

AVL *insertavl(AVL *avl, const char *id, Noeud **noeud) {
    if (!avl) {
        AVL *avltmp = malloc(sizeof(AVL));
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

Noeud *rechercheavl(AVL *avl, const char *id) {
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

Noeud *verifFichier(const char *fichier, const char *id_usine, AVL **index) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", fichier);
        return NULL;
    }

    char c1[64], c2[64], c3[64], c4[64], c5[64];
    Noeud *racine = NULL;

    while (fscanf(f, "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n",
                  c1, c2, c3, c4, c5) == 5) {

        // Trouver la racine (l'usine demandée en tant que parent OU enfant)
        if ((strcmp(c2, id_usine) == 0 || strcmp(c3, id_usine) == 0) && !racine) {
            Noeud *tmp;
            *index = insertavl(*index, id_usine, &tmp);
            racine = tmp;
        }

        // Ne traiter que les lignes concernant l'usine spécifiée (en c2 ou c3)
        if (strcmp(c2, id_usine) != 0 && strcmp(c3, id_usine) != 0)
            continue;
        
        // Récupérer ou créer le nœud parent
        Noeud *parent = rechercheavl(*index, c2);
        if (!parent) {
            *index = insertavl(*index, c2, &parent);
        }

        // Récupérer ou créer le nœud enfant
        Noeud *enfant = rechercheavl(*index, c3);
        if (!enfant) {
            *index = insertavl(*index, c3, &enfant);
        }

        // Définir le pourcentage de fuite
        if (strcmp(c5, "-") != 0)
            enfant->fuite = atof(c5);

        ajouteEnfants(parent, enfant);
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

void freeavl(AVL *avl) {
    if (!avl) return;
    freeavl(avl->fg);
    freeavl(avl->fd);
    free(avl);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s fichier.csv \"ID_USINE\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    AVL *index = NULL;
    Noeud *racine = verifFichier(argv[1], argv[2], &index);

    if (!racine) {
        printf("fuites = -1\n");
        freeavl(index);
        return EXIT_FAILURE;
    }

    // Volume de référence : 1 million de m3
    float volume_initial = 1000000.0f;
    float pertes = calculerFuites(racine, volume_initial);

    printf("Pertes totales pour l'usine %s : %.2f m³/an\n", argv[2], pertes);

    freearbre(racine);
    freeavl(index);
    return 0;
}