#ifndef COMPILATION_H
#define COMPILATION_H 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct delta{
		char caractere;
		int etat_prec;
		int etat_suiv;
	}DELTA;
typedef struct Automate_AFN{
	char* Alphabet;
	int s;
	int* F;
	int* Q;
	DELTA* tab_transition; 
}AFN;
	
AFN langage_vide();
AFN langage_mot_vide();
AFN langage_mot_caractere(char * mot);
void affichage_automate_AFN(AFN automate);


#endif
