#include"Principal.h"

File* incrémentationFICHIER( const char* nom ){
	File* f = fopen( nom , r );
	if( f == NULL ) exit(ERROR_FICHIER);
	
	
}
// Les fonctions suivantes vérifient les différentes lignes du fichier
// Cette fonction vérifie : SOURCE -> USINE
bool verif_S-U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont!=NULL && i->fuites!=NULL
}
// Cette fonction vérifie : USINE
bool verif_U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval==NULL && i->amont!=NULL && i->fuites==NULL
}
// Cette fonction vérifie : USINE->STOCKAGE
bool verif_U( Infos* i ){
	return i->id_usine==NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont==NULL && i->fuites!=NULL
}
// Cette fonction vérifie : STOCKAGE->JONCTION
bool verif_U( Infos* i ){
	return i->id_usine!=NULL && i->id_amont!=NULL && i->id_aval!=NULL && i->amont==NULL && i->fuites!=NULL
}
