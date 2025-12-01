#include "principal.h"

void leaks(pUsine u){
    if (u==NULL){
            printf("L'usine n'existe pas \n");
            return -1;
    }
    float leaktot=0;
    while(u!=NULL){
        leaktot+=u->vol*u->fuites;
        u=u->next;
    }
}