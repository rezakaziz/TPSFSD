#ifndef MACHINE_ABS_TOF_H_INCLUDED
#define MACHINE_ABS_TOF_H_INCLUDED
#include "constante.h"

//Ouvrir un fichier f, mode='a' pour ancien ou 'n' pour nouveau
void Ouvrir_TOF(TOF *f, char nomfichier[TAILLE_NOM], char mode);
//fermer le fichier f
void Fermer_TOF(FILE *f);
//lire le contenu de bloc i dans Buf
int LireDir_TOF(FILE *f,int i, Buffer buf);
//Ecrire le contenu de buffer dans le fichier au bloc i
int EcrireDir_TOF(FILE *f,int i, Buffer buf);
//lire le contenue de fichier sequentiellement
int LireSeq_TOF(FILE *F,Buffer buf );
//ecriture sequentiel dans le fichier
int EcrireSeq_TOF(FILE *F, Buffer buf );
//retourner la taille de la i eme caracteristique de fichier
int Entete_TOF( FILE *f, int i );
//affecter une valeur dans la ieme caractristique de fichier
int Aff_entete_TOF( FILE *f,int i,int val );
//Allouer un nouveau bloc dans un fichier TOF et retourne son numero
int allocbloc(FILE *f);

#endif // MACHINE_ABS_TOF_H_INCLUDED
