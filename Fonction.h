#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED
#include "constante.h"
int anneebissextile(int annee);

int setdate(int day,int month,int year);
date incremente(date Date);
void chargement_init(int anneedb,int anneefin);
void tabvilinit(char Ville[48][26]);
typedef struct caseindex
{
    char ville[TAILLE_NOM_VILLE];
    int nbloc[2000];
    int casevide;
}caseindex;
int rechercheindex(char nom[TAILLE_NOM_VILLE],caseindex index[49],int *trouv);
void creationindextof(TOF *fichier);



int str_istr (const char *cs, const char *ct);
char *str_sub (const char *s, int start, int endd);
void menu();
#endif // FONCTION_H_INCLUDED
