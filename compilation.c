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

// Copie les transitions de deux automates dans Automate
DELTA* Copy_transition(AFN automate1, AFN automate2, int* nbr){
	DELTA* tab = (DELTA*) malloc(sizeof(automate1.tailleTab_transit));
	int i,tailleTab=0;
	for(i=0; i<automate1.tailleTab_transit;i++) // Copie les transitions de  l'automate1
	{
		printf("testla0 \n");
		tab[i].caractere = automate1.tab_transition[i].caractere;
		tab[i].etat_prec = automate1.tab_transition[i].etat_prec;
		tab[i].etat_suiv= automate1.tab_transition[i].etat_suiv;
		tailleTab+=1;
	}
	int incr = 0;
	for(i=0; i<automate2.tailleTab_transit;i++) // Copie les transitions de  l'automate2 qui ne sont pas relies a l'etat initial
	{
		if(automate2.tab_transition[i].etat_prec != automate2.s){
			printf("testla2 \n");
			tab = realloc(tab, sizeof(DELTA)*(tailleTab + 1));
			tab[automate1.tailleTab_transit + incr].caractere = automate2.tab_transition[i].caractere;
			tab[automate1.tailleTab_transit + incr].etat_prec = automate2.tab_transition[i].etat_prec + automate1.tailleQ - 1;
			tab[automate1.tailleTab_transit + incr].etat_suiv= automate2.tab_transition[i].etat_suiv + automate1.tailleQ - 1;
			tailleTab+=1;
			incr += 1;
		}
	}
	printf(" (%d,%c,%d) \t \n", tab[tailleTab-1].etat_prec,tab[tailleTab-1].caractere,tab[tailleTab-1].etat_suiv);
	*nbr = tailleTab;
	return tab;
}

// Automate reconnaissant la concatenation des deux langages
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2){
	AFN Automate;
	Automate.tailleQ = 0;
	Automate.tailleF = 0;
	Automate.tailleTab_transit = 0;
	int i,j,k=0;
	int nbr; // contient le nombre de transitions copiees
	Automate.Q=malloc(sizeof(int)*(automate1.tailleQ + automate2.tailleQ - 1));
	//strcpy(Automate.Q, automate1.Q);
	for(i=0; i<(automate1.tailleQ); i++){
		Automate.Q[i] = automate1.Q[i];
		Automate.tailleQ += 1;
		}
	for(i=1; i<(automate2.tailleQ); i++){
		Automate.Q[automate1.tailleQ + i-1] = automate2.Q[i] + automate1.tailleQ - 1;
		Automate.tailleQ += 1;
		}
	for(j=0; j<(automate2.tailleF); j++){
		if(automate2.F[j] == automate2.s){ // si l'etat initial de l'automate2 est accepteur
			//printf(" bjr1 \n");
			Automate.F=malloc(sizeof(int)*automate1.tailleF);
			//strcpy(Automate.F, automate1.F);
			for(i=0; i<(automate1.tailleF); i++){
				Automate.F[i] = automate1.F[i];
				Automate.tailleF += 1;
				}
			for(i=0; i<automate2.tailleF; i++){
				Automate.F = realloc(Automate.F, sizeof(int));
				if(automate2.F[i] != 0){
					Automate.F[automate1.tailleF + i] = automate2.F[i] + automate1.tailleQ - 1;
					Automate.tailleF += 1;
				}
			}
			k=1;
			//strcat(Automate.F, automate2.F);
			}
		break;
	}
	if(k == 0){  // si l'etat initial de l'automate2 est non accepteur
		//printf(" HELLObjr2 \n");
		Automate.F = (int*)malloc((sizeof(int))*automate2.tailleF);
		//printf(" hello \n");
		//strcpy(Automate.F, automate2.F);
		for(i=0; i<(automate2.tailleF); i++){
			Automate.F[i] = automate2.F[i] + automate1.tailleQ - 1;
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
			tabAlpha = realloc(tabAlpha, sizeof(char));
			tabAlpha[i] = automate2.tab_transition[i].caractere;
			tabEtat = realloc(tabEtat, sizeof(int));
			tabEtat[i] = automate2.tab_transition[i].etat_suiv;
		}
	}
	Automate.tab_transition = malloc((sizeof(DELTA))*(automate1.tailleTab_transit));
	for(i=0; i<automate1.tailleTab_transit;i++) // Copie les transitions de  l'automate1
	{
		Automate.tab_transition[i].caractere = automate1.tab_transition[i].caractere;
		Automate.tab_transition[i].etat_prec = automate1.tab_transition[i].etat_prec;
		Automate.tab_transition[i].etat_suiv= automate1.tab_transition[i].etat_suiv;
		Automate.tailleTab_transit+=1;
	}
	int incr = 0;
	for(i=0; i<automate2.tailleTab_transit;i++) // Copie les transitions de  l'automate2 qui ne sont pas relies a l'etat initial
	{
		if(automate2.tab_transition[i].etat_prec != automate2.s){
			Automate.tab_transition = realloc(Automate.tab_transition, sizeof(DELTA)*(Automate.tailleTab_transit + 1));
			Automate.tab_transition[automate1.tailleTab_transit + incr].caractere = automate2.tab_transition[i].caractere;
			Automate.tab_transition[automate1.tailleTab_transit + incr].etat_prec = automate2.tab_transition[i].etat_prec + automate1.tailleQ - 1;
			Automate.tab_transition[automate1.tailleTab_transit + incr].etat_suiv= automate2.tab_transition[i].etat_suiv + automate1.tailleQ - 1;
			Automate.tailleTab_transit+=1;
			incr += 1;
		}
	}
	//Automate.tab_transition = Copy_transition(automate1, automate2, &nbr);
	//Automate.tailleTab_transit = nbr;
	nbr = Automate.tailleTab_transit;
	for(i=0; i<automate1.tailleF; i++){  // Ajout de nouvelles transitions a l'Automate
		for(j=0; j<strlen(tabAlpha); j++){
			Automate.tab_transition = realloc(Automate.tab_transition, sizeof(DELTA)*(Automate.tailleTab_transit + 1));
			Automate.tab_transition[nbr+i].caractere = tabAlpha[j];
			Automate.tab_transition[nbr+i].etat_prec = automate1.F[i];
			Automate.tab_transition[nbr+i].etat_suiv = tabEtat[j] + automate1.tailleQ - 1;
			Automate.tailleTab_transit += 1;
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

int verification_AFN(AFN automate) // verifi si un AFN est detreministe ou pas
 {
	int i,j,etat = 0;
	int nbr_elt = 0;
	char* tab = NULL;
	//tab non alloué
	for(i=0; i<(automate.tailleQ); i++){
		for(j=0; j<(automate.tailleTab_transit); j++){
			if(automate.Q[i] == automate.tab_transition[j].etat_prec){ // recopie toutes les carateres des transitions de l'etat i
				tab = realloc(tab, sizeof(char)*(nbr_elt + 1));
				tab[j] = automate.tab_transition[j].caractere;
				nbr_elt+=1;
				}
			}
		for(j=1; j<(nbr_elt); j++){
			if(tab[j-1] == tab[j]){
				etat = 1;
				}
			break;
		}
		if(etat != 0){
			break;
			}
	free(tab);
	}
	return etat;
}


/*
AFD creation_afd()
{
	AFD automate;
	automate.Alphabet=malloc(sizeof(char)*2);
	automate.Alphabet[0] = 'a';
	automate.Alphabet[1] = 'b';
	printf("alphabet ok \n");
	
	automate.F=malloc(sizeof(int));
	automate.F->valeur=2;
	automate.F->accepteur=1;
	automate.tailleF = 1;
	printf("etat final ok \n");

	automate.Q=malloc(sizeof(ETAT)*3);
	automate.tailleQ=3;
	automate.Q->tab_alphabet=(char*)malloc(sizeof(char)*3);
	automate.Q->tab_etat_suivant=(int*)malloc(sizeof(int)*3);

	
	ETAT etat_0,etat_1,etat_2;
	automate.Q[0]=etat_0;
	automate.Q[1]=etat_1;
	automate.Q[2]=etat_2;

	etat_0.valeur=0;
	etat_0.accepteur=0;
	etat_0.tab_alphabet[0]='a';
	etat_0.tab_etat_suivant[0]=1;
	etat_0.taille_transition=1;
	printf("etat 0 ok \n");

	etat_1.valeur=1;
	etat_1.accepteur=0;
	etat_1.tab_alphabet[0]='b';
	etat_1.tab_etat_suivant[0]=2;
	etat_1.taille_transition=1;
	printf("etat 1 ok \n");

	etat_2.valeur=2;
	etat_2.accepteur=1;
	etat_2.tab_alphabet[0]='a';
	etat_2.tab_etat_suivant[0]=2;
	etat_2.tab_alphabet[1]='b';
	etat_2.tab_etat_suivant[1]=2;
	etat_2.taille_transition=2;
	printf("etat 2 ok \n");
	

	return automate;

}
*/

/*
Bool reconnnaissance_mot(AFD automate, char* mot)
{
	Bool reconnu;
	ETAT etat_actuel=automate.s; //Le processus commence à l’état de départ q0
	for (int i=0; i < strlen(mot);i++) //Les symboles du mot sont lus les uns après les les autres.
	{
		etat_actuel.tab_etat_suivant[i]=mot[i];
	}

//À la lecture de chaque symbole, on emploie la fonction de transition δ pour se déplacer vers le prochain état (en utilisant l’état actuel et le caractère qui vient d’être lu).


	for (int k=0;k<automate.tailleF;k++) 
	{	
		if (etat_actuel.tab_etat_suivant[strlen(mot)]==automate.F->tab_alphabet[k])
		{
			reconnu=1; //le mot reconnu <=> le dernier état (qui lit le dernier caractère du mot) est un état de F.

			printf("mot reconnu \n");

		}
		else
		{
			reconnu=0;
			printf("mot non reconnu \n");
		}
	}
	return reconnu;
}
*/

/*
AFD determinisation (AFN automate)
{
	AFD automate_fd;

	//Les alphabets restent identiques
	automate_fd.Alphabet=malloc(sizeof(strlen(automate.Alphabet)));

	//Ensemble des nouveaux états
	automate_fd.Q=malloc(sizeof(int)*(automate.tailleQ)-1); //Q est initialisé à ∅

	ETAT* ensemble_etat;
	ETAT element,element_courant; //creation respectivement S et S'
	ensemble_etat[0].valeur=automate.s; //E un ensemble d’états initialisé à E = {{q0}}

	while(ensemble_etat!=NULL)
	{
		for (int i=0;i<ensemble_etat->taille_transition;i++)//Parcours de E
		{
			element=ensemble_etat[i]; //choisir un élément S de E (S est donc un sous ensemble de Qn)
			for (int j=0;j<automate_fd.tailleQ;j++)
			{
				automate_fd.Q[j]=element;//ajouter S à Qd
			}
			for (int k=0;k<strlen(automate.Alphabet);k++) //pour tout symbole a ∈ Σ
			{
				//calcul l’état S′ = ∪q ∈ S δn(q, a)
				element_courant.tab_alphabet=automate.tab_transition[k].caractere;
				//si S′ n’est pas déjà dans Qd, l’ajouter à E
				if (element_courant.valeur != automate_fd.Q)
				{
					for(int l=0;l<ensemble_etat->taille_transition;l++)
					{
						ensemble_etat[l]=element_courant;	
					}
				}
				else //ajouter un arc sur l’automate entre S et S′ et la valuer par a
				{
					element_courant.tab_alphabet=k
				}
			}
		}
	}
	return automate_fd;
}
*/



int main(int argc, char **argv)
{
	affichage_automate_AFN(langage_mot_caractere("aa"));
	printf("%d \n", verification_AFN(langage_mot_caractere("aaaaa")));
	//affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("aa"), langage_mot_caractere("bb")));
	//AFN automate_nd=creation_afn();
	//affichage_automate_AFN(automate_nd);
	//affichage_automate_AFN(langage_mot_caractere("aa"));
	//affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("a"), langage_mot_caractere("b")));
	//affichage_automate_AFN(FermetureIterrative_automate_standart(langage_mot_caractere("aaa")));
	return 0;
}

