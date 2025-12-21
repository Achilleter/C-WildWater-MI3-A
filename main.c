#include "Principal.h"

/* ============================================================================================
   FONCTION MAIN
   ============================================================================================ */
int main(int argc, char** argv) {
	
	clock_t start = clock(); // début chronométrage

	if (argc < 4) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		return 0;
	}
    const char* name = argv[1];      // nom fichier
    const char* fonction = argv[2];  // histo ou leaks
    const char* details = argv[3];   // type spécifique (max, src, real, id usine)

	
	
	if( strcmp(fonction, "leaks") == 0 ){
		faire_leak(name,details);
	}
	else if ( strcmp(fonction, "histo") == 0 &&
        (strcmp(details, "max") == 0 ||
         strcmp(details, "src") == 0 ||
         strcmp(details, "real") == 0) ){
		incrementationFICHIER(name, fonction , details );
	}
	
	clock_t end = clock(); // fin chronométrage
	double temps = (double)(end - start) / CLOCKS_PER_SEC;
	printf("Temps CPU : %.6f miliseconde\n", temps*1000);
	
	
	return 0;
}
