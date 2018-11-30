#ifndef MACHINE_ABS_TOVC_H_INCLUDED
#define MACHINE_ABS_TOVC_H_INCLUDED
#include "constante.h"

//Ouvrir un fichier f, mode='a' pour ancien ou 'n' pour nouveau
void ouvrir_TOVC(TOVC *f,char nomfichier[TAILLE_NOM],char mode);
//fermer le fichier f
void Fermer_TOVC(TOVC *f);
//lire le contenu de bloc i dans Buf
int LireDir_TOVC(TOVC *f,int i, Buffertovc *buf);
//Ecrire le contenu de buffer dans le fichier au bloc i
int EcrireDir_TOVC(TOVC *f,int i, Buffertovc buf);
//lire le contenue de fichier sequentiellement
int LireSeq_TOVC(TOVC *f, Buffertovc buf );
//ecriture sequentiel dans le fichier
int EcrireSeq_TOVC(TOVC *f, Buffertovc buf );
//retourner la taille de la i eme caracteristique de fichier
int Entete_TOVC( TOVC *f, int i );
//affecter une valeur dans la ieme caractristique de fichier
int aff_Entete_TOVC( TOVC *f, int i , int val);
//Allouer un nouveau bloc dans un fichier TOF et retourne son numero
int allocbloc_tovc(TOVC *f);


#endif // MACHINE_ABS_TOVC_H_INCLUDED
