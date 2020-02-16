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
DELTA* Copy_transition(AFN automate1, AFN automate2, int* nbr);
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2);
//AFN Reunion_automates_standards(AFN automate1, AFN automate2);
AFN FermetureIterrative_automate_standart(AFN automate);
AFN creation_afn();


/********************************************
 Automate fini déterministe
 * ******************************************/
int verification_AFN(AFN automate);
AFD creation_afd();
Bool reconnnaissance_mot(AFD automate, char* mot);

typedef struct Etat{
	int valeur;
	int accepteur;
	char* tab_alphabet;
	int* tab_etat_suivant;
	int taille_transition;

}ETAT;

typedef enum
{ False = 0, True = 1 }
Bool;

typedef struct Automate_AFD{
	ETAT s;
	char* Alphabet;
	ETAT* F;
	int tailleF;
	ETAT* Q;
	int tailleQ;
}AFD;


//copie AFN vers AFD
//verification AFN
//determinisation

#endif
