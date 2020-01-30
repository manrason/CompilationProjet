//#include <stdio.h>
#include "compilation.h"

AFN langage_vide(){
	AFN Automate;
	Automate.Alphabet=NULL;
	Automate.Q=NULL;
	Automate.F=NULL;
	Automate.tab_transition=NULL;
	return Automate;
}

AFN langage_mot_vide(){
	AFN Automate;
	Automate.Q= malloc(sizeof(int));
	Automate.Q[0]=0;
	Automate.F=malloc(sizeof(int));
	Automate.F[0]=0;
	Automate.s=0;
	Automate.tab_transition=NULL;
	Automate.Alphabet=NULL;
	return Automate;
}

AFN langage_mot_caractere(char * mot)
{
	AFN Automate;
	Automate.Q=malloc(sizeof(int)*2);
	Automate.Q[0]=0;
	Automate.Q[1]=1;
	Automate.F=malloc(sizeof(int));
	Automate.F[0]=1;
	Automate.s=0;
	Automate.Alphabet=malloc(sizeof(char));
	Automate.Alphabet[0] = mot[0];
	Automate.tab_transition = malloc(sizeof(DELTA));
	Automate.tab_transition[0].caractere = mot[0];
	Automate.tab_transition[0].etat_prec = 0;
	Automate.tab_transition[0].etat_suiv = 1;
	if ( strlen(mot) > 1){
		Automate.tab_transition = realloc(Automate.tab_transition, sizeof(DELTA));
		Automate.tab_transition[1].caractere = mot[0];
		Automate.tab_transition[1].etat_prec = 1;
		Automate.tab_transition[1].etat_suiv = 1;
		Automate.F=realloc(Automate.F, sizeof(int));
		Automate.F[1]=0;
		}
	return Automate;
}

void affichage_automate_AFN(AFN automate)
{
	printf("Etat initial = %d \n",automate.s);
	for (int i=0; i<((sizeof(automate.Q)/sizeof(int*)));i++)
	{
		printf("L'ensemble des etats = %d \t ",automate.Q[i]);
	}
	printf(" \n");
	for (int i=0; i<((sizeof(automate.F)/sizeof(int*)));i++)
	{
		printf("L'ensemble des etats finaux= %d \t ",automate.F[i]);
	}
	printf(" \n");
	for (int i=0; i<((sizeof(automate.tab_transition)/sizeof(DELTA*)));i++)
	{
		printf("L'ensemble des transitions = (%d,%c,%d) \t ",automate.tab_transition[i].etat_prec,automate.tab_transition[i].caractere,automate.tab_transition[i].etat_suiv);
	}
	printf(" \n");
	
	
}

int main(int argc, char **argv)
{
	/*int x;
	x=2;
	printf("%c", x);*/
	affichage_automate_AFN(langage_vide());
	return 0;
}

