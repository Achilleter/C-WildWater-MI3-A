#include "principal.h"

void leaks(Usines* u, float leaktot){//leaktot doit etre initialisé a 0 avant l'appel
    if (u==NULL){
            printf("L'usine n'existe pas \n");
            return -1;
    }
    while(u!=NULL){
        if(u->infos.fuites>0){
            leaktot+=u->infos.vol*u->(infos.fuites/100);
        }
        if(u->enfants.usine.enfants!=NULL){
            leaks(u->enfants.usine);
        }
        u=u->next;
    }
}