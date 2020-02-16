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
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2);
AFN Reunion_automates_standards(AFN automate1, AFN automate2);
AFN FermetureIterrative_automate_standart(AFN automate);
AFN creation_afn();


/********************************************
 Automate fini déterministe
 * ******************************************/

typedef struct Etat{
	int valeur;
	int accepteur;
	char* tab_alphabet;
	int* tab_etat_suivant;
	int taille_transition;

}ETAT;


typedef struct Automate_AFD{
	ETAT s;
	char* Alphabet;
	ETAT* F;
	int tailleF;
	ETAT* Q;
	int tailleQ;
}AFD;

void menu();
void choix_menu(char choix);
AFD creation_afd();
void affichage_automate_AFD(AFD automate);
int reconnnaissance_mot(AFD automate, char* mot);
//copie AFN vers AFD
int verification_AFN(AFN automate);
//determinisation

#endif
