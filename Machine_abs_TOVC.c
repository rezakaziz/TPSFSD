#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Machine_abs_TOVC.h"
#include "constante.h"
#include "Fonction.h"


void ouvrir_TOVC(TOVC *f,char nomfichier[TAILLE_NOM],char mode)
{
    f-> fichier=NULL;
    // ouverture du fichier au mode ancien
    if (mode=='a'|| mode=='A')
    {
        f-> fichier=fopen(nomfichier,"rb+");
        if (f->fichier!=NULL)
        {
            rewind(f->fichier);
            fread(&(f->Entete),sizeof(entetetovc),1,f->fichier);
        }
        else printf("Impossible d'ouvrir le fichier");
    }
     else if (mode=='N'||mode=='n')
    {
        f->fichier=fopen(nomfichier,"wb+");
        if (f->fichier!=NULL)
        {
            aff_Entete_TOVC(f,0,0);
            aff_Entete_TOVC(f,1,0);

        }
        else printf("impossible de creer le fichier ");
    }
    else printf("Mode d'ouverture incorrecte");
}
void Fermer_TOVC(TOVC *f)
// Fermeture d'un fichier
{
    if (f->fichier!=NULL)
    {
        Buffertovc buf;
        LireDir_TOVC(f,Entete_TOVC(f,0),&buf);
        aff_Entete_TOVC(f,1,(Entete_TOVC(f,0)-1)*taibuffer+strlen(buf.tab));
        rewind(f->fichier);
        fwrite(&(f->Entete),sizeof(entetetovc),1,f->fichier);
        fclose(f->fichier);
        f=NULL;
    }
}

// Lire_direct
int LireDir_TOVC(TOVC *f,int i, Buffertovc *buf)//return -1 si on a pas pu lire le bloc et 0 si on a lit un bloc
{
    if (f->fichier!=NULL)
    {
        if(Entete_TOVC(f,0)>=i)// le bloc qu'on veut le lire est inferieur ou egale au nb de blocs
        {
            fseek(f->fichier,sizeof(entetetovc)+((i-1)*sizeof(Buffertovc)),SEEK_SET);
            fread(buf,sizeof(Buffertovc),1,f->fichier);
            return 0;
        }

    }
    return -1;
}

// Ecire direct
int EcrireDir_TOVC(TOVC *f,int i, Buffertovc buf)
{
    if (f->fichier!=NULL)
    {
        if (Entete_TOVC(f,0)<i)
        {
            //la valeur de i on la fixe a nombre de bloc +1 quelque soit i initialement
            i=allocbloc_tovc(f);// di le nombre de bloc est superieur au nombre de bloc dans le fichier on doit allouer un nouveau blec et retournere son num
        }
        fseek(f->fichier,sizeof(entetetovc)+(i-1)*sizeof(Buffertovc),SEEK_SET);
        fwrite(&buf,sizeof(Buffertovc),1,f->fichier);
        return 0;
    }
    return -1;
}

// LECT SEQ
int LireSeq_TOVC(TOVC *f, Buffertovc buf )
{
    if (f->fichier!=NULL)
    {
        fread(&buf,sizeof(Buffertovc),1,f->fichier);

    }
    return 1;
}

//  ECRITURE SEQ


int EcrireSeq_TOVC(TOVC *f, Buffertovc buf )
{
    if (f->fichier!=NULL)
    {
        fwrite(&buf,sizeof(Buffertovc),1,f->fichier);
        //Aff_entete_TOVC(f,0,Entete_TOVC(f,0)+1);
        //Aff_entete_TOVC(f,1,Entete_TOVC(f,1)+buf.nbelement);
    }
    return 1;
}

// Caractéristiques de l'entete
int Entete_TOVC( TOVC *f, int i )
{
    if (f->fichier!=NULL)
    {
       switch (i)
       {
       case 0:
        return (f->Entete.ind_dernier_bloc);
        break;
       case 1:
        return (f->Entete.tailledefichier);
        break;
       default:
        printf("La caracteristique n'existe pas");
       }

    }
    return -1;
}

// Afficher l'entete

int aff_Entete_TOVC( TOVC *f, int i , int val)
{
    if (f->fichier!=NULL)
    {
       switch (i)
       {
       case 0:
         (f->Entete.ind_dernier_bloc)=val;
        break;
       case 1:
         (f->Entete.tailledefichier)=val;
        break;
       default:
        printf("La caracteristique n'existe pas");
       }

    }
    return -1;
}
/*
void Aff_Entete(TOVC * f)
{

    printf("  indice dernier bloc : %d\n",Entete(f,0));
    printf("  position libre dernier bloc : %d\n",Entete(f,1));
    printf("  Nombre caracteres inseres : %d\n",Entete(f,2));
    printf("  Nombre caracteres supprimes : %d\n",Entete(f,3));
}*/
/// Allouer un nouveau bloc
int allocbloc_tovc(TOVC *f)
{
    aff_Entete_TOVC(f,0,Entete_TOVC(f,0)+1);
    return Entete_TOVC(f,0);
}
