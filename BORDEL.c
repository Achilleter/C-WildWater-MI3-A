#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 64

/* ===================================================== */
/* ================= STRUCTURES ========================= */
/* ===================================================== */

typedef struct Node {
    char id[MAX_ID];
    float fuite;                 // % fuite du tronçon entrant
    struct Node *enfants;        // premier enfant
    struct Node *next;           // frère suivant
} Node;

/* ================= AVL ================= */

typedef struct AVL {
    char id[MAX_ID];
    Node *node;
    int height;
    struct AVL *left;
    struct AVL *right;
} AVL;

/* ===================================================== */
/* ================= AVL UTILITAIRES ==================== */
/* ===================================================== */

int height(AVL *n) {
    return n ? n->height : 0;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVL *rotate_right(AVL *y) {
    AVL *x = y->left;
    AVL *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVL *rotate_left(AVL *x) {
    AVL *y = x->right;
    AVL *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int balance_factor(AVL *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

/* ===================================================== */
/* ================= AVL INSERT / FIND ================= */
/* ===================================================== */

Node *create_node(const char *id) {
    Node *n = malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Erreur allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->id, id, MAX_ID - 1);
    n->id[MAX_ID - 1] = '\0';
    n->fuite = 0.0f;
    n->enfants = NULL;
    n->next = NULL;
    return n;
}

AVL *avl_insert(AVL *root, const char *id, Node **out_node) {
    if (!root) {
        AVL *n = malloc(sizeof(AVL));
        if (!n) {
            fprintf(stderr, "Erreur allocation mémoire AVL\n");
            exit(EXIT_FAILURE);
        }
        n->node = create_node(id);
        strncpy(n->id, id, MAX_ID - 1);
        n->id[MAX_ID - 1] = '\0';
        n->left = n->right = NULL;
        n->height = 1;
        *out_node = n->node;
        return n;
    }

    if (strcmp(id, root->id) < 0)
        root->left = avl_insert(root->left, id, out_node);
    else if (strcmp(id, root->id) > 0)
        root->right = avl_insert(root->right, id, out_node);
    else {
        *out_node = root->node;
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));
    int bf = balance_factor(root);

    // Rotation droite
    if (bf > 1 && strcmp(id, root->left->id) < 0)
        return rotate_right(root);
    
    // Rotation gauche
    if (bf < -1 && strcmp(id, root->right->id) > 0)
        return rotate_left(root);
    
    // Double rotation gauche-droite
    if (bf > 1 && strcmp(id, root->left->id) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    
    // Double rotation droite-gauche
    if (bf < -1 && strcmp(id, root->right->id) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

Node *avl_find(AVL *root, const char *id) {
    if (!root) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp == 0) return root->node;
    if (cmp < 0) return avl_find(root->left, id);
    return avl_find(root->right, id);
}

/* ===================================================== */
/* ================= GRAPHE ============================= */
/* ===================================================== */

void add_child(Node *parent, Node *child) {
    child->next = parent->enfants;
    parent->enfants = child;
}

/* ===================================================== */
/* ================= PARSING CSV ======================== */
/* ===================================================== */

Node *build_graph(const char *file, const char *id_usine, AVL **index) {
    FILE *f = fopen(file, "r");
    if (!f) {
        fprintf(stderr, "Erreur ouverture fichier %s\n", file);
        return NULL;
    }

    char c1[64], c2[64], c3[64], c4[64], c5[64];
    Node *racine = NULL;

    while (fscanf(f, "%63[^;];%63[^;];%63[^;];%63[^;];%63[^\n]\n",
                  c1, c2, c3, c4, c5) == 5) {

        // Trouver la racine (l'usine demandée en tant que parent OU enfant)
        if ((strcmp(c2, id_usine) == 0 || strcmp(c3, id_usine) == 0) && !racine) {
            Node *tmp;
            *index = avl_insert(*index, id_usine, &tmp);
            racine = tmp;
        }

        // Ne traiter que les lignes concernant l'usine spécifiée (en c2 ou c3)
        if (strcmp(c2, id_usine) != 0 && strcmp(c3, id_usine) != 0)
            continue;
        
        // Récupérer ou créer le nœud parent
        Node *parent = avl_find(*index, c2);
        if (!parent) {
            *index = avl_insert(*index, c2, &parent);
        }

        // Récupérer ou créer le nœud enfant
        Node *child = avl_find(*index, c3);
        if (!child) {
            *index = avl_insert(*index, c3, &child);
        }

        // Définir le pourcentage de fuite
        if (strcmp(c5, "-") != 0)
            child->fuite = atof(c5);

        add_child(parent, child);
    }

    fclose(f);
    return racine;
}

/* ===================================================== */
/* ================= CALCUL DES FUITES ================= */
/* ===================================================== */

int count_children(Node *n) {
    int c = 0;
    for (Node *e = n->enfants; e; e = e->next) c++;
    return c;
}

float compute_leaks(Node *n, float volume) {
    if (!n || volume <= 0) return 0;

    // Calculer le volume perdu par fuite
    float perdu = volume * n->fuite / 100.0f;
    float restant = volume - perdu;

    int nb = count_children(n);
    float total = perdu;

    // Si pas d'enfants, retourner juste les fuites de ce nœud
    if (nb == 0) return total;

    // Distribuer équitablement le volume restant aux enfants
    float part = restant / nb;
    for (Node *e = n->enfants; e; e = e->next)
        total += compute_leaks(e, part);

    return total;
}

/* ===================================================== */
/* ================= LIBÉRATION ========================= */
/* ===================================================== */

void free_graph(Node *n) {
    if (!n) return;
    for (Node *e = n->enfants; e; ) {
        Node *tmp = e;
        e = e->next;
        free_graph(tmp);
    }
    free(n);
}

void free_avl(AVL *a) {
    if (!a) return;
    free_avl(a->left);
    free_avl(a->right);
    free(a);
}

/* ===================================================== */
/* ================= MAIN =============================== */
/* ===================================================== */

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s fichier.csv \"ID_USINE\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    AVL *index = NULL;
    Node *racine = build_graph(argv[1], argv[2], &index);

    if (!racine) {
        printf("Usine non trouvée ou erreur de lecture\n");
        free_avl(index);
        return EXIT_FAILURE;
    }

    // Volume de référence : 1 million de m3
    float volume_initial = 1000000.0f;
    float pertes = compute_leaks(racine, volume_initial);

    printf("Pertes totales pour l'usine %s : %.2f m³/an\n", argv[2], pertes);

    free_graph(racine);
    free_avl(index);
    return 0;
}