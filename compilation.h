#ifndef COMPILATION_H
#define COMPILATION_H 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/********************************************
 Automate fini non déterministe
 * ******************************************/


typedef struct delta{
		char caractere;
		char etat_prec;
		char etat_suiv;
	}DELTA;
typedef struct Automate_AFN{
	char* Alphabet;
	int s;
	int* F;
	int tailleF;
	int* Q;
	int tailleQ;
	DELTA* tab_transition;
	int tailleTab_transit;
}AFN;
	
AFN langage_vide();
AFN langage_mot_vide();
AFN langage_mot_caractere(char * mot);
void affichage_automate_AFN(AFN automate);
DELTA* Copy_transition(AFN automate1, AFN automate2);
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2);
AFN FermetureIterrative_automate_standart(AFN automate);


/********************************************
 Automate fini déterministe
 * ******************************************/




#endif
