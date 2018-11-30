#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constante.h"
#include "Machine_abs_TOF.h"

//Ouvrir un fichier f, mode='a' pour ancien ou 'n' pour nouveau
void Ouvrir_TOF(TOF *f, char nomfichier[TAILLE_NOM], char mode)
{
    f->fichier=NULL;
    if (mode=='A'|| mode=='a')
    {
        f->fichier=fopen(nomfichier,"rb+");
        if (f->fichier!=NULL)
        {


            rewind(f->fichier);
            fread(&f->Entete,sizeof(entete),1,f->fichier);
        }
        else printf("Impossible d'ouvrir le fichier");
    }

    else if (mode=='N'||mode=='n')
    {
        f->fichier=fopen(nomfichier,"wb+");
        if (f->fichier!=NULL)
        {
            Aff_entete_TOF(f,0,0);
            Aff_entete_TOF(f,1,0);
        }
        else printf("impossible de creer le fichier ");
    }
    else printf("Mode d'ouverture incorrecte");
}


//fermer le fichier f
void Fermer_TOF(TOF *f)
{
    if (f->fichier!=NULL)
    {
        rewind(f->fichier);
        fwrite(&(f->Entete),sizeof(entete),1,f->fichier);
        fclose(f->fichier);
        f=NULL;
    }
}

//lire le contenu de bloc i dans Buf
int LireDir_TOF(TOF *f,int i, Buffer *buf)//return -1 si on a pas pu lire le bloc et 0 si on a lit un bloc
{
    if (f->fichier!=NULL)
    {
        if(Entete_TOF(f,0)>=i)
        {
            fseek(f->fichier,sizeof(entete)+((i-1)*sizeof(Buffer)),SEEK_SET);
            fread(buf,sizeof(Buffer),1,f->fichier);
            return 0;
        }

    }
    return -1;
}

//Ecrire le contenu de buffer dans le fichier au bloc i//les bloc sont numeroté a partir de 1
int EcrireDir_TOF(TOF *f,int i, Buffer buf)
{
    if (f->fichier!=NULL)
    {
        if (Entete_TOF(f,0)<i)
        {
            //la valeur de i on la fixe a nombre de bloc +1 quelque soit i initialement
            i=allocbloc(f);// di le nombre de bloc est superieur au nombre de bloc dans le fichier on doit allouer un nouveau blec et retournere son num
        }
        fseek(f->fichier,sizeof(entete)+(i-1)*sizeof(Buffer),SEEK_SET);
        fwrite(&buf,sizeof(Buffer),1,f->fichier);
        Aff_entete_TOF(f,1,Entete_TOF(f,1)+1);
        return 0;
    }
    return -1;
}

//lecture sequentiel dans un fichier
int LireSeq_TOF(TOF *f,Buffer *buf )
{
    fread(buf,sizeof(Buffer),1,f->fichier);
    return 0;
}
int EcrireSeq_TOF(TOF *f, Buffer buf )
{
    if (f->fichier!=NULL)
    {
        fwrite(&buf,sizeof(Buffer),1,f->fichier);
        Aff_entete_TOF(f,0,Entete_TOF(f,0)+1);
        Aff_entete_TOF(f,1,Entete_TOF(f,1)+buf.nbelement);
    }
    return 1;
}

//retourner la taille de la i eme caracteristique de fichier
int Entete_TOF( TOF *f, int i )
{
    if (f->fichier!=NULL)
    {
       switch (i)
       {
       case 0:
        return (f->Entete).nbbloc;
        break;
       case 1:
        return (f->Entete).cpt_insert;
        break;
       default:
        printf("La caracteristique n'existe pas");
       }

    }
    return -1;
}

int Aff_entete_TOF( TOF *f,int i,int val )
{
    if (f->fichier!=NULL)
    {
       switch (i)
       {
       case 0:
        (f->Entete).nbbloc=val;
        break;
       case 1:
        (f->Entete).cpt_insert=val;
        break;
       default:
        printf("La caracteristique n'existe pas");
       }

    }
    return -1;
}


int allocbloc(TOF *f)
{
    Aff_entete_TOF(f,0,Entete_TOF(f,0)+1);
    return Entete_TOF(f,0);
}
