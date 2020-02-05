//#include <stdio.h>
#include "compilation.h"

// Automate standart reconnaissant le langage vide
AFN langage_vide(){
	AFN Automate;
	Automate.Alphabet=NULL;
	Automate.Q=NULL;
	Automate.F=NULL;
	Automate.tab_transition=NULL;
	return Automate;
}

// Automate standart reconnaissant le langage compose d'un seul mot vide
AFN langage_mot_vide(){
	AFN Automate;
	Automate.Q= malloc(sizeof(char));
	Automate.Q[0]='0';
	Automate.F=malloc(sizeof(char));
	Automate.F[0]='0';
	Automate.s='0';
	Automate.tab_transition=NULL;
	Automate.Alphabet=NULL;
	return Automate;
}

// Automate standart reconnaissant le langage compose d'un mot d'un caractere
AFN langage_mot_caractere(char * mot)
{
	AFN Automate;
	Automate.Q=malloc(sizeof(char)*2);
	Automate.Q[0]='0';
	Automate.Q[1]='1';
	Automate.F=malloc(sizeof(char));
	Automate.F[0]='1';
	Automate.s='0';
	Automate.Alphabet=malloc(sizeof(char));
	Automate.Alphabet[0] = mot[0];
	Automate.tab_transition = malloc(sizeof(DELTA));
	Automate.tab_transition[0].caractere = mot[0];
	Automate.tab_transition[0].etat_prec = '0';
	Automate.tab_transition[0].etat_suiv = '1';
	if ( strlen(mot) > 1){
		Automate.tab_transition = realloc(Automate.tab_transition, sizeof(DELTA));
		Automate.tab_transition[1].caractere = mot[0];
		Automate.tab_transition[1].etat_prec = '1';
		Automate.tab_transition[1].etat_suiv = '1';
		Automate.F=realloc(Automate.F, sizeof(char));
		Automate.F[1]='0';
		//printf("(%c,%c,%c) \t ",Automate.tab_transition[1].etat_prec,Automate.tab_transition[1].caractere,Automate.tab_transition[1].etat_suiv);
		}
	return Automate;
}

// Affiche automate fini non deterministe
void affichage_automate_AFN(AFN automate)
{
	printf("Etat initial = %c \n",automate.s);
	printf("L'ensemble des etats = ");
	for (int i=0; i<strlen(automate.Q); i++)
	{
		printf("%c \t ",automate.Q[i]);
	}
	printf(" \n");
	printf("L'ensemble des etats finaux= ");
	for (int i=0; i<strlen(automate.F);i++)
	{
		printf("%c \t ",automate.F[i]);
	}
	printf(" \n");
	printf("L'ensemble des transitions = ");
	for (int i=0; i<=((sizeof(automate.tab_transition)/sizeof(DELTA*)));i++)
	{
		printf(" (%c,%c,%c) \t ", automate.tab_transition[i].etat_prec,automate.tab_transition[i].caractere,automate.tab_transition[i].etat_suiv);
	}
	printf(" \n");
	
	
}

// Copie les transitions d'un automate dans un autre
DELTA* Copy_transition(AFN automate1, AFN automate2){
	DELTA* tab = (DELTA*) malloc((sizeof(automate1.tab_transition)/sizeof(DELTA*))*2);
	int i;
	for(i=0; i<=(sizeof(automate1.tab_transition)/sizeof(DELTA*));i++)
	{
		tab[i].caractere = automate1.tab_transition[i].caractere;
		tab[i].etat_prec = automate1.tab_transition[i].etat_prec;
		tab[i].etat_suiv= automate1.tab_transition[i].etat_suiv;
	}
	for(i=(sizeof(automate1.tab_transition)/sizeof(DELTA*)); i<=((sizeof(automate2.tab_transition)/sizeof(DELTA*)) + (sizeof(automate1.tab_transition)/sizeof(DELTA*)));i++)
	{
		if(automate2.s != automate2.tab_transition[i].caractere){
			tab[i].caractere = automate2.tab_transition[i].caractere;
			tab[i].etat_prec = automate2.tab_transition[i].etat_prec;
			tab[i].etat_suiv= automate2.tab_transition[i].etat_suiv;
		}
	}
	return tab;
}

// Automate reconnaissant la concatenation des deux langages
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2){
	AFN Automate;
	int tailleQ = strlen(automate1.Q) + strlen(automate2.Q);
	int tailleF = strlen(automate1.F) + strlen(automate2.F);
	int tailleAlphabet = strlen(automate1.Alphabet) + strlen(automate2.Alphabet);
	//int tailleTab_transitions = sizeof((automate1.tab_transition)/sizeof(DELTA*)) + (sizeof(automate2.tab_transition)/sizeof(DELTA*));
	Automate.Q=malloc(sizeof(char)*tailleQ);
	strcpy(Automate.Q, automate1.Q);
	strcat(Automate.Q, automate2.Q);
	Automate.F=malloc(sizeof(char)*tailleF);
	if(strchr(automate2.F, automate2.s) != NULL){ // si l'etat initial de l'automate2 est accepteur
		strcpy(Automate.F, automate1.F);
		strcat(Automate.F, automate2.F);
		}else{ // si l'etat initial de l'automate2 est non accepteur
			strcpy(Automate.F, automate2.F);
			}
	Automate.s=automate1.s;
	Automate.Alphabet=malloc(sizeof(tailleAlphabet));
	strcpy(Automate.Alphabet, automate1.Alphabet);
	strcat(Automate.Alphabet, automate2.Alphabet);
	Automate.tab_transition = malloc(sizeof(DELTA)*(sizeof(automate1.tab_transition)/sizeof(DELTA*)));
	Automate.tab_transition = Copy_transition(automate1, automate1);
	/*Automate.tab_transition[0].caractere = mot[0];
	Automate.tab_transition[0].etat_prec = 0;
	Automate.tab_transition[0].etat_suiv = 1;*/
	
	return Automate;
}

int main(int argc, char **argv)
{
	/*int x;
	x=2;
	printf("%c", x);*/
	affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("aa"), langage_mot_caractere("bb")));
	return 0;
}

