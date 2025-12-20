#include "Principal.h"

int taille(AVLeak *arbre) {//Donne la hauteur de l'arbre
    if (arbre != NULL) {
        return arbre->h;
    }
    else {
        return 0;
    }
}

AVLeak *rotation_droite(AVLeak *racine) {//Rotation droite des avleaks
    AVLeak *nouvelle_racine;
    AVLeak *sous_arbre_droit;
    // Le fils gauche devient la nouvelle racine 
    nouvelle_racine = racine->fg;
    // On conserve le sous-arbre droit de la nouvelle racine 
    sous_arbre_droit = nouvelle_racine->fd;
    // Rotation 
    nouvelle_racine->fd = racine;
    racine->fg = sous_arbre_droit;
    // Mise à jour des hauteurs
    racine->h = max(taille(racine->fg), taille(racine->fd)) + 1;
    nouvelle_racine->h = max(taille(nouvelle_racine->fg), taille(nouvelle_racine->fd)) + 1;
    return nouvelle_racine;
}

AVLeak *rotation_gauche(AVLeak *racine) {//Rotation gauche des avleaks
    AVLeak *nouvelle_racine;
    AVLeak *sous_arbre_gauche;
// Le fils droit devient la nouvelle racine 
    nouvelle_racine = racine->fd;
// On conserve le sous-arbre gauche de la nouvelle racine 
    sous_arbre_gauche = nouvelle_racine->fg;
// Rotation
    nouvelle_racine->fg = racine;
    racine->fd = sous_arbre_gauche;
// Mise à jour des hauteurs
    racine->h = max(taille(racine->fg), taille(racine->fd)) + 1;
    nouvelle_racine->h = max(taille(nouvelle_racine->fg), taille(nouvelle_racine->fd)) + 1;
    return nouvelle_racine;
}

int equilibre(AVLeak *arbre) {//renvoie le facteur d'équilibre
    if (arbre != NULL) {
        return taille(arbre->fg) - taille(arbre->fd);
    }
    else{
        return 0;
    }
}

Noeud *creernoeud(const char *id) {//initialisation d'un noeud d'arbre
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

AVLeak *rotation(AVLeak *avl, const char *id) {//effectue les rotations nécessaires pour équilibrer l'AVLeak
    avl->h = 1 + max(taille(avl->fg), taille(avl->fd));
    int h = equilibre(avl);
    // Rotation droite
    if (h > 1 && strcmp(id, avl->fg->id) < 0) {
        return rotation_droite(avl);
    }
    // Rotation gauche
    if (h < -1 && strcmp(id, avl->fd->id) > 0) {
        return rotation_gauche(avl);
    }
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

AVLeak *insertavl(AVLeak *avl, const char *id, Noeud **noeud) {//Insertion dans l'AVLeak
    if (!avl) {
        AVLeak *avltmp = malloc(sizeof(AVLeak));
        if (!avltmp) {
            printf("Erreur allocation mémoire \n");
            exit(EXIT_FAILURE);
        }
        //création du noeud
        avltmp->noeud = creernoeud(id);
        strncpy(avltmp->id, id, MAX_ID - 1);
        avltmp->id[MAX_ID - 1] = '\0';
        avltmp->fg = NULL;
        avltmp->fd = NULL;
        avltmp->h = 1;
        *noeud = avltmp->noeud;
        return avltmp;
    }
//recherche de la position d'insertion
    if (strcmp(id, avl->id) < 0) {
        avl->fg = insertavl(avl->fg, id, noeud);
    }
    else if (strcmp(id, avl->id) > 0) {
        avl->fd = insertavl(avl->fd, id, noeud);
    }
    else {
        *noeud = avl->noeud;
        return avl;
    }
//équilibrage de l'AVLeak
    avl=rotation(avl,id);
    return avl;
}

Noeud *rechercheavl(AVLeak *avl, const char *id) {//Recherche d'un id dans l'AVLeak
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

void ajouteEnfants(Noeud *parent, Noeud *enfant) {//Ajoute un enfant à un noeud parent de l'arbre passé en paramètre
    enfant->next = parent->enfants;
    parent->enfants = enfant;
}

Noeud *verifFichier(const char *fichier, const char *id_usine, AVLeak **index, float *vol_init) {//Ouverture et verification du fichier
    FILE *f = fopen(fichier, "r");
    if (!f) {
        return NULL;
    }
// Initialisation des variables de lecture
    char c1[64], c2[64], c3[64], c4[64], c5[64];
    Noeud *racine = NULL;
// Lecture ligne par ligne puis ajout deans les variables appropriées
    while (fscanf(f, "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n", c1, c2, c3, c4, c5) == 5) {
        Infos i = {"", "", "", -1, -1};
        strcpy(i.id_usine, c1);
        strcpy(i.id_amont, c2);
        strcpy(i.id_aval,  c3);
        i.vol    = strcmp(c4, "-") ? atof(c4) : -1;
        i.fuites = strcmp(c5, "-") ? atof(c5) : -1;
        // pour récupérer le volume initial de l'usine
        if (verif_S_U(&i) && strcmp(c3, id_usine) == 0){
            *vol_init = i.vol*(1 - i.fuites/100);
        }
        Noeud *amont = NULL, *aval = NULL;
// Insertion des noeuds dans l'AVLeak selon le type d'embranchement
        if (strcmp(c2, "-") != 0){
            *index = insertavl(*index, c2, &amont);
        }
        if (strcmp(c3, "-") != 0){
            *index = insertavl(*index, c3, &aval);
        }

        if (amont && aval) {
            ajouteEnfants(amont, aval);
            if (i.fuites >= 0) {
                amont->fuite = i.fuites;
            }
        }
// Détermination de l'id de la racine
        if (!racine && strcmp(c3, id_usine) == 0) {
            racine = aval;
        }
        if (!racine && strcmp(c2, id_usine) == 0) {
            racine = amont;
        }
    }
// Fermeture du fichier et retour de la racine
    fclose(f);
    return racine;
}


int nbrEnfant(Noeud *n) {//Compte le nombre d'enfants d'un noeud
    int i = 0;
    Noeud *e = n->enfants;
    while (e != NULL) {
        i++;
        e = e->next;
    }
    return i;
}

float calculerFuites(Noeud *n, float volume) {//Fonction principale de calcul des fuites
    if (!n || volume <= 0) {//Verification de la validité du noeud et du volume
        return 0;
    }
    // Calculer le volume perdu par les fuites
    float perdu = volume * n->fuite / 100.0f;
    float restant = volume - perdu;
    int nb = nbrEnfant(n);
    float total = perdu;
    // Si pas d'enfants, retourner juste les fuites de ce nœud
    if (nb == 0) {
        return total;
    }
    // Distribuer équitablement le volume restant aux enfants
    float part = restant / nb;
    // Rappeler récursivement pour chaque enfant et frère
    for (Noeud *e = n->enfants; e; e = e->next){
        total += calculerFuites(e, part);
    }
    return total;
}

void freearbre(Noeud *n) {//fonction de libération de la mémoire de l'arbre
    if (!n) {
        return;
    }
    for (Noeud *e = n->enfants; e; ) {
        Noeud *tmp = e;
        e = e->next;
        freearbre(tmp);
    }
    free(n);
}

void freeavl(AVLeak *avl) {//fonction de libération de la mémoire de l'AVLeak
    if (!avl) {
        return;
    }
    freeavl(avl->fg);
    freeavl(avl->fd);
    free(avl);
}

void faire_leak(const char* nomFICHIER, const char* nomUSINE) {//Fonction principale de gestion des fuites
//initialisation des variables puis récupération des arbres
    AVLeak *index = NULL;
	float volume_initial = 0;
	Noeud *racine = verifFichier(nomFICHIER, nomUSINE, &index, &volume_initial);
//vérification de la validité de la racine (et donc de l'arbre)
	if (!racine) {
		printf("fuites = -1\n");
		freeavl(index);
		return exit(EXIT_FAILURE);
	}
//calcul des pertes totales à partir des arbres
	float pertes = calculerFuites(racine, volume_initial);
	printf("Pertes totales pour l'usine %s : %.2f m³/an\n", nomUSINE, pertes);
	freearbre(racine);
	freeavl(index);
}