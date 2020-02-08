#ifndef COMPILATION_H
#define COMPILATION_H 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct delta{
		char caractere;
		char etat_prec;
		char etat_suiv;
	}DELTA;
typedef struct Automate_AFN{
	char* Alphabet;
	char s;
	char* F;
	char* Q;
	DELTA* tab_transition; 
}AFN;
	
AFN langage_vide();
AFN langage_mot_vide();
AFN langage_mot_caractere(char * mot);
void affichage_automate_AFN(AFN automate);
DELTA* Copy_transition(AFN automate1, AFN automate2);
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2);
AFN Reunion_automates_standards(AFN automate1, AFN automate2);


#endif
