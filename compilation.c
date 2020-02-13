//#include <stdio.h>
#include "compilation.h"


/********************************************
 Automate fini non déterministe
 * ******************************************/

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
	Automate.Q= malloc(sizeof(int));
	Automate.Q[0]=0;
	Automate.tailleQ = 1; 
	Automate.F=malloc(sizeof(int));
	Automate.F[0]=0;
	Automate.tailleF = 1;
	Automate.s=0;
	Automate.tab_transition=NULL;
	Automate.Alphabet=NULL;
	return Automate;
}

// Automate standart reconnaissant le langage compose d'un mot d'un caractere
AFN langage_mot_caractere(char * mot)
{
	AFN Automate;
	Automate.F=malloc(sizeof(int));
	Automate.F[0]=strlen(mot);
	Automate.tailleF = 1;
	Automate.s=0;
	Automate.Alphabet=malloc(sizeof(char));
	Automate.Alphabet[0] = mot[0];
	Automate.tab_transition = malloc(sizeof(DELTA)*strlen(mot));
	int i;
	Automate.Q=malloc(sizeof(int));
	Automate.Q[0]=0;
	Automate.tailleQ = strlen(mot) + 1;
	Automate.tailleTab_transit = strlen(mot);
	
	for(i=0; i<strlen(mot); i++){
		Automate.tab_transition[i].caractere = mot[0];
		Automate.tab_transition[i].etat_prec = i;
		Automate.tab_transition[i].etat_suiv = i+1;
		Automate.Q[i+1]=i+1;
		}
	printf("\n");
	return Automate;
}

// Affiche automate fini non deterministe
void affichage_automate_AFN(AFN automate)
{
	printf("Etat initial = %d \n",automate.s);
	printf("L'ensemble des etats = ");
	for (int i=0; i<automate.tailleQ; i++)
	{
		printf("%d \t ",automate.Q[i]);
	}
	printf(" \n");
	printf("L'ensemble des etats finaux= ");
	for (int i=0; i<automate.tailleF;i++)
	{
		printf("%d \t ",automate.F[i]);
	}
	printf(" \n");
	printf("L'ensemble des transitions = ");
	for (int i=0; i<automate.tailleTab_transit ;i++)
	{
		printf(" (%d,%c,%d) \t ", automate.tab_transition[i].etat_prec,automate.tab_transition[i].caractere,automate.tab_transition[i].etat_suiv);
	}
	printf(" \n");
	
}

// Copie les transitions de deux automates dans un nouveau
DELTA* Copy_transition(AFN automate1, AFN automate2){
	DELTA* tab = (DELTA*) malloc(sizeof(automate1.tailleTab_transit));
	int i,tailleTab=0;
	for(i=0; i<automate1.tailleTab_transit;i++) // Copie les transitions de  l'automate1
	{
		printf("testla \n");
		tab[i].caractere = automate1.tab_transition[i].caractere;
		tab[i].etat_prec = automate1.tab_transition[i].etat_prec;
		tab[i].etat_suiv= automate1.tab_transition[i].etat_suiv;
		tailleTab+=1;
	}
	for(i=0; i<automate2.tailleTab_transit;i++) // Copie les transitions de  l'automate2 qui ne sont pas relies a l'etat initial
	{
		if(automate2.tab_transition[i].etat_prec != automate2.s){
			//printf("testla2 \n");
			tab = realloc(tab, sizeof(DELTA));
			tab[automate1.tailleTab_transit + i].caractere = automate2.tab_transition[i].caractere;
			tab[automate1.tailleTab_transit + i].etat_prec = automate2.tab_transition[i].etat_prec;
			tab[automate1.tailleTab_transit + i].etat_suiv= automate2.tab_transition[i].etat_suiv;
			tailleTab+=1;
		}
	}
	return tab;
}

// Automate reconnaissant la concatenation des deux langages
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2){
	AFN Automate;
	Automate.tailleQ = 0;
	Automate.tailleF = 0;
	Automate.tailleTab_transit = 0;
	int i,j,k=0;
	Automate.Q=malloc(sizeof(int)*(automate1.tailleQ + automate2.tailleQ - 1));
	//strcpy(Automate.Q, automate1.Q);
	for(i=0; i<(automate1.tailleQ); i++){
		Automate.Q[i] = automate1.Q[i];
		Automate.tailleQ += 1;
		}
	for(i=0; i<(automate2.tailleQ); i++){
		Automate.Q[automate1.tailleQ + i] = automate2.Q[i+1];
		Automate.tailleQ += 1;
		}
	for(j=0; j<(automate2.tailleF); j++){
		if(automate2.F[j] == automate2.s){ // si l'etat initial de l'automate2 est accepteur
			printf(" bjr1 \n");
			Automate.F=malloc(sizeof(int)*automate1.tailleF);
			//strcpy(Automate.F, automate1.F);
			for(i=0; i<(automate1.tailleF); i++){
				Automate.F[i] = automate1.F[i];
				Automate.tailleF += 1;
				}
			for(i=0; i<automate2.tailleF; i++){
				Automate.F = realloc(Automate.F, sizeof(int));
				Automate.F[automate1.tailleF] = automate2.F[i+1];
				Automate.tailleF += 1;
				}
			k=1;
			//strcat(Automate.F, automate2.F);
			}
		break;
		printf(" HELLObjr0 \n");
	}
	if(k == 0){  // si l'etat initial de l'automate2 est non accepteur
		printf(" HELLObjr2 \n");
		Automate.F = (int*)malloc((sizeof(int))*automate2.tailleF);
		printf(" hello \n");
		//strcpy(Automate.F, automate2.F);
		for(i=0; i<(automate2.tailleF); i++){
			Automate.F[i] = automate2.F[i];
			Automate.tailleF += 1;
			}
		}
	Automate.s=automate1.s;
	Automate.Alphabet=malloc(sizeof(strlen(automate1.Alphabet) + strlen(automate2.Alphabet)));
	strcpy(Automate.Alphabet, automate1.Alphabet);
	strcat(Automate.Alphabet, automate2.Alphabet);
	char *tabAlpha = NULL;
	int *tabEtat = NULL;
	for (i=0; i<automate2.tailleTab_transit;i++)  // recherche l'alphabet ayant des transitions avec l'etat inititial de l'automate2
	{
		if(automate2.tab_transition[i].etat_prec == automate2.s){
			printf(" bjr3 \n");
			tabAlpha = realloc(tabAlpha, sizeof(char));
			tabAlpha[i] = automate2.tab_transition[i].caractere;
			tabEtat = realloc(tabEtat, sizeof(int));
			tabEtat[i] = automate2.tab_transition[i].etat_suiv;
		}
	}
	Automate.tab_transition = malloc((sizeof(DELTA))*(automate1.tailleTab_transit));
	Automate.tailleTab_transit = automate1.tailleTab_transit;
	Automate.tab_transition = Copy_transition(automate1, automate2);
	for(i=0; i<automate1.tailleF; i++){  // Ajout de nouvelles transitions a l'Automate
		printf(" bjr41 \n");
		for(j=0; j<strlen(tabAlpha); j++){
			printf(" bjr4 \n");
			Automate.tab_transition[Automate.tailleTab_transit+i].caractere = tabAlpha[j];
			Automate.tab_transition[Automate.tailleTab_transit+i].etat_prec = automate1.F[i];
			Automate.tab_transition[Automate.tailleTab_transit+i].etat_suiv = tabEtat[j];
			}
		}
	
	return Automate;
}
/*
//Automate reconnaissant la réunion des deux langages
AFN Reunion_automates_standards(AFN automate1, AFN automate2)
{
	AFN Automate;
	Automate.tailleQ = 0;
	Automate.tailleF = 0;
	Automate.tailleTab_transit = 0;
	
	//Etat initial
	Automate.s=1;

	//Ensemble des états
	Automate.Q=malloc(sizeof(int)*(automate1.tailleQ + automate2.tailleQ - 1));
	Automate.Q=automate1.Q - automate1.s + automate2.Q - automate2.s + Automate.s;
	
	//Ensemble des états finaux
	if (strchr(automate1.F,automate1.s)!=NULL & (strchr(automate2.F,automate2.s)) !=NULL) // si l'état initial de l'automate1 et l'automate2 sont accepteurs
	{
		strcpy(Automate.F,automate1.F);
		strcat(Automate.F,automate2.F);
		strcat(Automate.F,&Automate.s); // F1\s1 U F2\s2 U s

	}
	else{ // si l'état initial de l'automate 1 et 2 ne sont pas accepteurs
		strcpy(Automate.F,automate1.F);
		strcat(Automate.F,automate2.F);
	}

	//Alphabet
	Automate.Alphabet=malloc(sizeof(strlen(automate1.Alphabet) ));//+ strlen(automate2.Alphabet)));
	strcpy(Automate.Alphabet, automate1.Alphabet);
	//strcat(Automate.Alphabet, automate2.Alphabet);
	
	//Ensemble des transitions
	Automate.tab_transition = malloc(sizeof(DELTA)*(sizeof(automate1.tab_transition)+ 1)); //sizeof(automate2.tab_transition)));
	Automate.tailleTab_transit=automate1.tailleTab_transit+automate2.tailleTab_transit;
	for (int i=0; i<Automate.tailleTab_transit;i++)  // rajoute de nouvelles transitions dans l'automate
	{
		for (int j=0; j<Automate.tailleF;j++)
		{
			Automate.tab_transition = realloc(Automate.tab_transition, sizeof(DELTA));
			Automate.tab_transition[i].caractere = Automate.tab_transition[i].caractere;
			Automate.tab_transition[i].etat_prec = Automate.F[j];
			Automate.tab_transition[i].etat_suiv = Automate.tab_transition[i].etat_suiv;
			Automate.tailleTab_transit += 1;
		}
		
	}
	Automate.tab_transition=automate1.tab_transition;


	return Automate;
}


// Automate reconnaissant la fermeture iterrative d'un automate
AFN FermetureIterrative_automate_standart(AFN automate){
	int i,j,k;
	k = automate.tailleTab_transit;
	for (i=0; i<k;i++)  // rajoute de nouvelles transitions dans l'automate
	{
		if(automate.tab_transition[i].etat_prec == automate.s){
			printf(" bjr3 \n");
			for (j=0; j<automate.tailleF;j++)
			{
				automate.tab_transition = realloc(automate.tab_transition, sizeof(DELTA));
				automate.tab_transition[k+j].caractere = automate.tab_transition[i].caractere;
				automate.tab_transition[k+j].etat_prec = automate.F[j];
				automate.tab_transition[k+j].etat_suiv = automate.tab_transition[i].etat_suiv;
				automate.tailleTab_transit += 1;
			}
		}
	}
	k=0;
	for(j=0; j<(automate.tailleF); j++){
		if(automate.F[j] == automate.s){ // si l'etat initial de l'automate est accepteur
			printf(" bjr1 \n");
			k=1;
			break;
			}
	}
	if(k == 0){  // si l'etat initial de l'automate est non accepteur
		printf(" bjr2 \n");
		automate.F=realloc(automate.F, sizeof(int));
		automate.F[automate.tailleF]=0;
		automate.tailleF += 1;
		}
	return automate;
}
*/
/********************************************
 Creation automate fini non-déterministe
 * ******************************************/
AFN creation_afn()
{
	AFN Automate;
	Automate.F=malloc(sizeof(int));
	Automate.F[0]=3;
	Automate.tailleF = 1;
	Automate.s=0;
	Automate.Alphabet=malloc(sizeof(char)*2);
	Automate.Alphabet[0] = 'a';
	Automate.Alphabet[1] = 'b';
	Automate.tab_transition = malloc(sizeof(DELTA)*6);
	Automate.Q=malloc(sizeof(int));
	Automate.Q[0]=0;
	Automate.Q[1]=1;
	Automate.Q[2]=2;
	Automate.Q[3]=3;
	Automate.tailleQ = 4;
	Automate.tailleTab_transit = 6;
	Automate.tab_transition[0].caractere = 'a';
	Automate.tab_transition[0].etat_prec = 0;
	Automate.tab_transition[0].etat_suiv = 0;

	Automate.tab_transition[1].caractere = 'b';
	Automate.tab_transition[1].etat_prec = 0;
	Automate.tab_transition[1].etat_suiv = 0;

	Automate.tab_transition[2].caractere = 'a';
	Automate.tab_transition[2].etat_prec = 0;
	Automate.tab_transition[2].etat_suiv = 1;

	Automate.tab_transition[3].caractere = 'a';
	Automate.tab_transition[3].etat_prec = 1;
	Automate.tab_transition[3].etat_suiv = 2;

	Automate.tab_transition[4].caractere = 'b';
	Automate.tab_transition[4].etat_prec = 1;
	Automate.tab_transition[4].etat_suiv = 2;

	Automate.tab_transition[5].caractere = 'a';
	Automate.tab_transition[5].etat_prec = 2;
	Automate.tab_transition[5].etat_suiv = 3;
	printf("\n");
	return Automate;
}





/********************************************
 Automate fini déterministe
 * ******************************************/






int main(int argc, char **argv)
{
	AFN automate_nd=creation_afn();
	affichage_automate_AFN(automate_nd);
	//affichage_automate_AFN(langage_mot_caractere("aa"));
	//affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("a"), langage_mot_caractere("b")));
	//affichage_automate_AFN(FermetureIterrative_automate_standart(langage_mot_caractere("aaa")));
	return 0;
}

