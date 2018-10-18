#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constante.h"
#include "Machine_abs_TOF.h"

//Ouvrir un fichier f, mode='a' pour ancien ou 'n' pour nouveau
void Ouvrir_TOF(FILE *f, char nomfichier[TAILLE_NOM], char mode)
{
    f=NULL;
    if (mode=='A'|| mode=='a')f=fopen(nomfichier,"rb+");
    else if (mode=='N'||mode=='n')f=fopen(nomfichier,"wb+");
    else printf("Mode d'ouverture incorrecte");
}


//fermer le fichier f
void Fermer_TOF(FILE *f)
{
    fclose(f);
    f=NULL;
}
