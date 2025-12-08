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