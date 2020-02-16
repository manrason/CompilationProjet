//#include <stdio.h>
#include "compilation.h"

#define TAILLE_MAX_BUFFER 100

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
	//printf("\n");
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

// Automate reconnaissant la concatenation des deux langages
AFN Concatenation_automates_standarts(AFN automate1, AFN automate2){
	AFN Automate;
	Automate.tailleQ = 0;
	Automate.tailleF = 0;
	Automate.tailleTab_transit = 0;
	int i,j,k=0;
	int nbr; // contient le nombre de transitions copiees
	Automate.Q=malloc(sizeof(int)*(automate1.tailleQ + automate2.tailleQ - 1));
	for(i=0; i<(automate1.tailleQ); i++){
		Automate.Q[i] = automate1.Q[i];
		Automate.tailleQ += 1;
		}
	for(i=0; i<(automate2.tailleQ); i++){
		if(automate2.Q[i] != 0){
			Automate.Q[automate1.tailleQ + k] = automate2.Q[i] + automate1.tailleQ - 1;
			Automate.tailleQ += 1;
			k += 1;
			}
		}
	k=0;

	for(j=0; j<(automate2.tailleF); j++){
		if(automate2.F[j] == automate2.s){ // si l'etat initial de l'automate2 est accepteur
			Automate.F=malloc(sizeof(int)*automate1.tailleF);
			for(i=0; i<(automate1.tailleF); i++){
				Automate.F[i] = automate1.F[i];
				Automate.tailleF += 1;
				}
			for(i=0; i<automate2.tailleF; i++){
				Automate.F = realloc(Automate.F, sizeof(int)*(Automate.tailleF + 1));
				if(automate2.F[i] != 0){
					Automate.F[automate1.tailleF + i] = automate2.F[i] + automate1.tailleQ - 1;
					Automate.tailleF += 1;
				}
			}
			k=1;
			}
		break;
	}
	if(k == 0){  // si l'etat initial de l'automate2 est non accepteur
		Automate.F = (int*)malloc((sizeof(int))*automate2.tailleF);
		for(i=0; i<(automate2.tailleF); i++){
			Automate.F[i] = automate2.F[i] + automate1.tailleQ - 1;
			Automate.tailleF += 1;
			}
		}
	Automate.s=automate1.s;
	Automate.Alphabet=malloc(sizeof(strlen(automate1.Alphabet) + strlen(automate2.Alphabet)));
	strcpy(Automate.Alphabet, automate1.Alphabet);
	k=0;
	for(int i=0; i<(strlen(automate2.Alphabet)); i++){
		if(strchr(Automate.Alphabet,automate2.Alphabet[i]) == NULL){
			Automate.Alphabet=realloc(Automate.Alphabet, sizeof(strlen(Automate.Alphabet) + 1));
			Automate.Alphabet[strlen(automate1.Alphabet) + k] = automate2.Alphabet[i];
			k += 1;
			}
		}
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
	free(tabAlpha);
	free(tabEtat);
	return Automate;
}

//Automate reconnaissant la réunion des deux langages
AFN Reunion_automates_standards(AFN automate1, AFN automate2)
{
	AFN Automate;
	Automate.tailleQ = 0;
	Automate.tailleF = 0;
	Automate.tailleTab_transit = 0;
	
	//Etat initial
	Automate.s=0;

	//Ensemble des états
	Automate.Q=malloc(sizeof(int)*(automate1.tailleQ + automate2.tailleQ - 2));
	int k = 0;
	for(int i=0; i<(automate1.tailleQ); i++){
			Automate.Q[i] = automate1.Q[i];
			Automate.tailleQ += 1;
		}
	for(int i=0; i<(automate2.tailleQ); i++){
		if(automate2.Q[i] != 0){
			Automate.Q[automate1.tailleQ + k] = automate2.Q[i] + automate1.tailleQ - 1;
			Automate.tailleQ += 1;
			k += 1;
			}
		}
	
	//Ensemble des états finaux
	Automate.F=malloc(sizeof(int)*(automate1.tailleF));
	for(int i=0; i<(automate1.tailleF); i++){
		Automate.F[i] = automate1.F[i];
		Automate.tailleF += 1;
		}
	k = 0;
	for(int i=0; i<(automate2.tailleF); i++){
		if(automate2.F[i] != 0){
			Automate.F=realloc(Automate.F, sizeof(int)*(Automate.tailleF + 1));
			Automate.F[automate1.tailleF + k] = automate2.F[i] + automate1.tailleQ - 1;
			Automate.tailleF += 1;
			k += 1;
			}
		}

	//Alphabet
	Automate.Alphabet=malloc(sizeof(strlen(automate1.Alphabet)));
	strcpy(Automate.Alphabet, automate1.Alphabet);
	k=0;
	for(int i=0; i<(strlen(automate2.Alphabet)); i++){
		if(strchr(Automate.Alphabet,automate2.Alphabet[i]) == NULL){
			Automate.Alphabet=realloc(Automate.Alphabet, sizeof(strlen(Automate.Alphabet) + 1));
			Automate.Alphabet[strlen(automate1.Alphabet) + k] = automate2.Alphabet[i];
			k += 1;
			}
		}
	
	//Ensemble des transitions
	Automate.tailleTab_transit=automate1.tailleTab_transit+automate2.tailleTab_transit;
	Automate.tab_transition = malloc(sizeof(DELTA)*(sizeof(Automate.tailleTab_transit)));
	char *tabAlpha = NULL;
	int *tabEtat = NULL;
	int incr = 0;
	for(int i=0; i<automate1.tailleTab_transit;i++) // Copie les transitions de  l'automate1 qui ne sont pas relies a l'etat initial
	{
		if(automate1.tab_transition[i].etat_prec != automate1.s){
			Automate.tab_transition[incr].caractere = automate1.tab_transition[i].caractere;
			Automate.tab_transition[incr].etat_prec = automate1.tab_transition[i].etat_prec;
			Automate.tab_transition[incr].etat_suiv= automate1.tab_transition[i].etat_suiv;
			incr += 1;
		}
	}
	for(int i=0; i<automate2.tailleTab_transit;i++) // Copie les transitions de  l'automate2 qui ne sont pas relies a l'etat initial
	{
		if(automate2.tab_transition[i].etat_prec != automate2.s){
			Automate.tab_transition[incr].caractere = automate2.tab_transition[i].caractere;
			Automate.tab_transition[incr].etat_prec = automate2.tab_transition[i].etat_prec + automate1.tailleQ - 1;
			Automate.tab_transition[incr].etat_suiv= automate2.tab_transition[i].etat_suiv + automate1.tailleQ - 1;
			incr += 1;
		}
	}
	for (int i=0; i<automate1.tailleTab_transit;i++)  // recherche l'alphabet ayant des transitions avec l'etat inititial de l'automate1
	{
		if(automate1.tab_transition[i].etat_prec == automate1.s){
			tabAlpha = realloc(tabAlpha, sizeof(char));
			tabAlpha[i] = automate1.tab_transition[i].caractere;
			tabEtat = realloc(tabEtat, sizeof(int));
			tabEtat[i] = automate1.tab_transition[i].etat_suiv;
		}
	}
	for(int j=0; j<strlen(tabAlpha); j++){  // Ajout de nouvelles transitions a l'Automate
		Automate.tab_transition[incr].caractere = tabAlpha[j];
		Automate.tab_transition[incr].etat_prec = Automate.s;
		Automate.tab_transition[incr].etat_suiv = tabEtat[j];
		incr += 1;
		}
	free(tabAlpha);
	free(tabEtat);
	for (int i=0; i<automate2.tailleTab_transit;i++)  // recherche l'alphabet ayant des transitions avec l'etat inititial de l'automate2
	{
		if(automate2.tab_transition[i].etat_prec == automate2.s){
			tabAlpha = realloc(tabAlpha, sizeof(char));
			tabAlpha[i] = automate2.tab_transition[i].caractere;
			tabEtat = realloc(tabEtat, sizeof(int));
			tabEtat[i] = automate2.tab_transition[i].etat_suiv;
		}
	}
	for(int j=0; j<strlen(tabAlpha); j++){  // Ajout de nouvelles transitions a l'Automate
		Automate.tab_transition[incr].caractere = tabAlpha[j];
		Automate.tab_transition[incr].etat_prec = Automate.s;
		Automate.tab_transition[incr].etat_suiv = tabEtat[j] + automate1.tailleQ - 1;
		incr += 1;
		}
	free(tabAlpha);
	free(tabEtat);

	return Automate;
}


// Automate reconnaissant la fermeture iterrative d'un automate
AFN FermetureIterrative_automate_standart(AFN automate){
	int i,j,k;
	k = automate.tailleTab_transit;
	for (i=0; i<k;i++)  // rajoute de nouvelles transitions dans l'automate
	{
		if(automate.tab_transition[i].etat_prec == automate.s){ //parcours l'ensemble des transitions ayant pour etat precedent l'etat initial
			for (j=0; j<automate.tailleF;j++) //parcours l'ensemble des etats accepteurs
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
			k=1;
			break;
			}
	}
	if(k == 0){  // si l'etat initial de l'automate est non accepteur
		automate.F=realloc(automate.F, sizeof(int));
		automate.F[automate.tailleF]=0;
		automate.tailleF += 1;
		}
	return automate;
}
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

int verification_AFN(AFN automate) // verifi si un AFN est detreministe ou pas (retourne 0 si deterministe et 1 sinon)
 {
	int i,j,etat = 0;
	int nbr_elt = 0;
	char* tab = NULL;
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

AFD creation_afd()
{
	AFD automate;
	automate.Alphabet=malloc(sizeof(char)*2);
	automate.Alphabet[0] = 'a';
	automate.Alphabet[1] = 'b';
	//printf("alphabet ok \n");
	
	ETAT etat_0,etat_1,etat_2;
	etat_0.valeur=0;
	etat_0.accepteur=0;
	etat_0.tab_alphabet = malloc(sizeof(char));
	etat_0.tab_alphabet[0]='a';
	etat_0.tab_etat_suivant = malloc(sizeof(int));
	etat_0.tab_etat_suivant[0]=1;
	etat_0.taille_transition=1;
	//printf("etat 0 ok \n");
	etat_1.valeur=1;
	etat_1.accepteur=0;
	etat_1.tab_alphabet = malloc(sizeof(char));
	etat_1.tab_alphabet[0]='b';
	etat_1.tab_etat_suivant = malloc(sizeof(int));
	etat_1.tab_etat_suivant[0]=2;
	etat_1.taille_transition=1;
	//printf("etat 1 ok \n");
	etat_2.valeur=2;
	etat_2.accepteur=1;
	etat_2.tab_alphabet = malloc(sizeof(char)*2);
	etat_2.tab_etat_suivant = malloc(sizeof(int)*2);
	etat_2.tab_alphabet[0]='a';
	etat_2.tab_etat_suivant[0]=2;
	etat_2.tab_alphabet[1]='b';
	etat_2.tab_etat_suivant[1]=2;
	etat_2.taille_transition=2;
	//printf("etat 2 ok \n");
	
	automate.Q=malloc(sizeof(ETAT)*3);
	automate.tailleQ=3;
	automate.Q[0]=etat_0;
	automate.Q[1]=etat_1;
	automate.Q[2]=etat_2;
	
	automate.F=malloc(sizeof(ETAT));
	automate.F[0] = etat_2;
	automate.tailleF = 1;
	//printf("etat final ok \n");
	
	automate.s = etat_0;
	
	return automate;
}
// Affiche automate fini deterministe
void affichage_automate_AFD(AFD automate){
	printf("Etat initial = %d \n",automate.s.valeur);
	printf("L'ensemble des etats = ");
	for (int i=0; i<automate.tailleQ; i++)
	{
		printf("%d \t ",automate.Q[i].valeur);
	}
	printf(" \n");
	printf("L'ensemble des etats finaux= ");
	for (int i=0; i<automate.tailleF;i++)
	{
		printf("%d \t ",automate.F[i].valeur);
	}
	printf(" \n");
	printf("L'ensemble des transitions = ");
	for (int i=0; i<automate.tailleQ ;i++)
	{
		for (int j=0; j<automate.Q[i].taille_transition ;j++)
		{
			printf(" (%d,%c,%d) \t ", automate.Q[i].valeur, automate.Q[i].tab_alphabet[j] ,automate.Q[i].tab_etat_suivant[j]);
		}
	}
	printf(" \n");
	
}


int reconnnaissance_mot(AFD automate, char* mot)
{
	int reconnu=0,accepte=1;
	ETAT etat_actuel=automate.s; //Le processus commence à l’état de départ q0
	
	int compteur=0;
	for (int i=0; i<strlen(mot) ;i++)
	{
		for (int j=0; j<etat_actuel.taille_transition ;j++)
		{
			if( etat_actuel.tab_alphabet[j] == mot[compteur])
			{
				etat_actuel=automate.Q[etat_actuel.tab_etat_suivant[j]];
				compteur+=1;
				reconnu=1;
				break;
			}
			
		}
		if (reconnu!=1){
			accepte=0;
			break;
		}
		
	}
	if(etat_actuel.accepteur!=1) //on verifie qu'au dernier caractere du mot, l'etat actuel soit un etat accepteur
	{
		accepte=0;
	}
	return accepte;
}

/*
ETAT copie_etat_afn_etat_afd(AFN automate, int indice)
{
	
	ETAT etat_0;
	etat_0.tab_alphabet=NULL;
	etat_0.tab_etat_suivant=NULL;
	etat_0.taille_transition=0;
	etat_0.valeur=automate.Q[indice];
	etat_0.accepteur=0;
	
	for(int i=0;i<automate.tailleF;i++)
	{
		if (etat_0.valeur==automate.F[i])
		{
			etat_0.accepteur=1;
			break;
		}
	}
	
	for (int i=0; i<automate.tailleTab_transit;i++)  
	{
		if(automate.tab_transition[i].etat_prec == automate.Q[indice])
		{
			etat_0.tab_alphabet=realloc(etat_0.tab_alphabet,sizeof(char)*(etat_0.strlen(etat_0.tab_alphabet)+1));
			etat_0.tab_etat_suivant=realloc(etat_0.tab_etat_suivant,sizeof(int)*(etat_0.taille_transition+1));
			etat_0.tab_alphabet[i]=automate.tab_transition[i].caractere;
			etat_0.tab_etat_suivant[i]=automate.tab_transition[i].etat_suiv;
			etat_0.taille_transition+=1;
		}
	}
	return etat_0;
}

AFD determinisation (AFN automate)
{
	AFD automate_fd;
	automate_fd.tailleF=0;
	automate_fd.tailleQ=0;
	automate_fd.Q=malloc(sizeof(ETAT));
	
	
	ETAT etat_0, etat_nouveau; //creation de S et S'
	etat_0=copie_etat_afn_etat_afd(automate,O);
	
	
	ETAT* ensemble_etat;
	ensemble_etat=malloc(sizeof(ETAT));
	ensemble_etat[0]=etat_0;
	int taille_tab_etat=1;
	
	automate_fd.Q[0]=etat_0; // ajout de S a Qd
	automate_fd.tailleQ+=1;
	
	
	for (int i=0; i< strlen(automate.Alphabet);i++)  //pour tout symbole de l'alphabet
	{
		for(int k=0;k< (strlen(etat_0.tab_alphabet);k++)
		{
			if(etat_0.tab_alphabet[k]==automate.Alphabet[i])
			{
				etat_nouveau=copie_etat_afn_etat_afd(automate,etat_0.tab_etat_suivant[k]);
				break;
			}
			
		}
		int reconnu=1;
		for (int l=0;l<automate_fd.tailleQ;l++)
		{
			if (etat_nouveau.valeur==automate_fd.Q[l]) //si S' est dans Qd
			{
				reconnu=0;
			}
		}
		if (reconnu==1)
		{
			ensemble_etat=realloc(ensemble_etat,sizeof(ETAT)*taille_tab_etat+1);
			ensemble_etat[taille_tab_etat]=etat_nouveau;
			taille_tab_etat+=1;
		}
		free (etat_0.tab_alphabet);
		free (etat_0.tab_etat_suivant);
		etat_0.tailleTab_transit=0;
		
		//creation arc entre S et S' par le symbole lu de l'alphabet
		etat_0.tab_alphabet=malloc(sizeof(char));
		etat_0.tab_etat_suivant=malloc(sizeof(int));
		etat_0.tab_alphabet[0]=automate.Alphabet[i];
		etat_0.tab_etat_suivant[0]=etat_nouveau.valeur;
		etat_0.tailleTab_transit+=1;
		
	}
*/		
	
	
/*void choix_menu(char choix){
	char chaine[10];
	char chaine1[10];
	char chaine2[10];
	printf(" jdjgdgdggdgd %c hdhsghghdghsghsd \n", choix);
	switch(choix){
		case '1':
			affichage_automate_AFN(langage_vide());
			break;
		case '2':
			affichage_automate_AFN(langage_mot_vide());
			break;
		case '3':
			printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine);
			affichage_automate_AFN(langage_mot_caractere(chaine));
			break;
		case '4':
			printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine1);
			printf("\n entrer une deuxieme chaine d'un seul caratere:  ");
			scanf(" %s ", chaine2);
			affichage_automate_AFN(Reunion_automates_standards(langage_mot_caractere(chaine1), langage_mot_caractere(chaine2)));
			break;
		case '5':
			printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine1);
			printf("\n entrer une deuxieme chaine d'un seul caratere:  ");
			scanf(" %s ", chaine2);
			affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere(chaine1), langage_mot_caractere(chaine2)));
			break;
		case '6':
			printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine);
			affichage_automate_AFN(FermetureIterrative_automate_standart(langage_mot_caractere(chaine1)));
			break;
		case '9':
			exit(0);
		default:
			system("clear");
			menu();
		}
	}

void menu(){
	char ch;
	printf("\t********************* PROJET COMPILATION ********************\n\n");
	printf("\t 1. langage vide \n \t 2. langage mot vide \n \t 3. langage mot d'n seul caractere \n \t 4. Reunion_automates_standard \n \t 5. Concatenation_automates_standarts \n \t 6. FermetureIterrative_automate_standart \n \t 7. \n \t 8. \n \t 9. Quitter");
	printf("\n \t\t\t Entrer votre choix: ");
	scanf(" %c ", &ch);
	printf(" jdjgdgdggdgd %c hdhsghghdghsghsd \n", ch);
	choix_menu(ch);
}*/

int main(int argc, char **argv)
{
	/*long choix;
	char lettre;
	char buffer[TAILLE_MAX_BUFFER];
	printf("\t********************* PROJET COMPILATION ********************\n\n");
	printf("(0) Quitter\n(1) Langage vide\n(2) Mot vide\n(3) Un mot\n");
	printf("(4) Réunion\n(5) Concaténation\n(6) Mise à l'étoile\n");
	printf("(7) Exécution d'un mot\n(8) Déterminisation\n(9) Minimisation\n");		printf("\n \t\t\t Entrer votre choix: ");
	fgets(buffer,TAILLE_MAX_BUFFER,stdin);
	choix = atoi(buffer);
	do{
		
		char chaine[10];
		char chaine1[10];
		char chaine2[10];
		switch(choix){
			case 0: //quitter
				break;
			case 1: //Langage vide
				affichage_automate_AFN(langage_vide());
				break;
			case 2: //Mot vide
				affichage_automate_AFN(langage_mot_vide());
				break;
			case 3: //Un mot
				printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine);
			affichage_automate_AFN(langage_mot_caractere(chaine));
				break;				
			case 4://réunion
					printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine1);
			printf("\n entrer une deuxieme chaine d'un seul caratere:  ");
			scanf(" %s ", chaine2);
			affichage_automate_AFN(Reunion_automates_standards(langage_mot_caractere(chaine1), langage_mot_caractere(chaine2)));
				break;
					
			case 5: //concaténation
					printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine1);
			printf("\n entrer une deuxieme chaine d'un seul caratere:  ");
			scanf(" %s ", chaine2);
			affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere(chaine1), langage_mot_caractere(chaine2)));
				break;
			case 6: //mise à l'étoile
			printf("\n entrer une chaine d'un seul caratere:  ");
			scanf(" %s ", chaine);
			affichage_automate_AFN(FermetureIterrative_automate_standart(langage_mot_caractere(chaine1)));
				break;
				
			case 7: //exécution d'un mot
				printf(" (valeur 1 pour mot accepte et 0 dans le cas contraire) %d \n", reconnnaissance_mot(creation_afd(),"abbabb"));	
				break;
				
			case 8: //déterminisation
					
				break;
				
			case 9: //minimisation
					
				break;
			default:
				printf("veillez choisir un numero de la liste\n");
				
				break;
		}
	} while(choix != 0);*/
	//affichage_automate_AFD(creation_afd());
	//printf(" (valeur 1 pour mot accepte et 0 dans le cas contraire) %d \n", reconnnaissance_mot(creation_afd(),"abbabb"));
	//affichage_automate_AFN(langage_mot_caractere("aa"));
	//printf("%d \n", verification_AFN(langage_mot_caractere("aaaaa")));
	//affichage_automate_AFN(Reunion_automates_standards(langage_mot_caractere("aa"), langage_mot_caractere("bb")));
	//affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("a"), langage_mot_caractere("b")));
	//AFN automate_nd=creation_afn();
	//affichage_automate_AFN(automate_nd);
	//affichage_automate_AFN(langage_mot_caractere("aa"));
	//affichage_automate_AFN(Concatenation_automates_standarts(langage_mot_caractere("aa"), langage_mot_caractere("bb")));
	//affichage_automate_AFN(FermetureIterrative_automate_standart(creation_afn()));
	return 0;
}

