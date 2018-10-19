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
//lire le contenu de bloc i dans Buf
int LireDir_TOF(FILE *f,int i, Buffer buf)//return -1 si on a pas pu lire le bloc et 0 si on a lit un bloc
{
    if (f!=NULL)
    {
        if(Entete_TOF(f,1)>i)
        {
            fseek(f,sizeof(entete)+i*sizeof(Buffer),SEEK_SET);
            fread(&buf,sizeof(Buffer),1,f);
            return 0;
        }

    }
    return -1;
}

//Ecrire le contenu de buffer dans le fichier au bloc i
int EcrireDir_TOF(FILE *f,int i, Buffer buf)
{
    if (f!=NULL)
    {
        fseek(f,sizeof(entete)+i*sizeof(Buffer),SEEK_SET);
        fwrite(&buf,sizeof(Buffer),1,f);
        Aff_entete_TOF(f,1,Entete_TOF(f,1)+1);
    }
}


//retourner la taille de la i eme caracteristique de fichier
int Entete_TOF( FILE *f, int i )
{
    entete Entetef;
    if (f!=NULL)
    {

    }
}
