#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Machine_abs_TOF.h"
#include "constante.h"
#include "Fonction.h"

int anneebissextile(int annee)//pour traiter le cas de fevrier contient 29 jour
{
    if (annee%4==0)
    {
        if (annee%100!=0)
        {

                return 1;
        }
        else
        {
            if(annee%400==0)
            {
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

int setdate(int day,int month,int year)//verifier si la date est correcte return 0 si la date est fausse et 1 si elle est correcte
{
    if (month>0 && month<13)//si le mois est entre janvier et decembre
    {
    switch(month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (day<1 || day >31)//cas de faux date
        {
            return 0;
        }
        break;
    case 2:
        if (anneebissextile(year))//cas fevrier a 29 jour
        {
            if (day<1 || day >29)//cas faux date
            {
                return 0;
            }

        }
        else//cas fevrier a 28 jour
        {
            if (day<1 || day>28)//cas faux date
            {
               return 0;
            }
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day<1 || day >30)//cas faux date
        {
            return 0;
        }
        break;
    default://cas date correcte
        return 1;


    }
    }
    else return 0;//cas faux date
    return 1;
}

date incremente(date Date)//incrementer la date
{
    switch(Date.mois)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
        //cas des mois 31 jour
        if (Date.jour<31)//si on est pas arriver au 31eme jour
        {
             Date.jour++;//
             return Date;
        }
        else
        {
            Date.jour=1;
            Date.mois++;
            return Date;
        }
        break;
    case 12:
        if (Date.jour<31)
        {
             Date.jour++;
            return Date;
        }
        else
        {
            Date.jour=1;
            Date.mois=1;
            Date.annee++;
                return Date;
        }
        break;

    case 2://cas fevrier
        if (anneebissextile(Date.annee))//cas annees bessextile
        {
            if (Date.jour<29)
            {
                Date.jour++;
                return Date;
            }
            else
            {
                Date.jour=1;
                Date.mois++;
                return Date;
            }
        }
        else//annee non bessextile
        {
            if (Date.jour<28)
            {
                Date.jour++;
                return Date;
            }
            else
            {
                Date.jour=1;
                Date.mois++;
                return Date;
            }
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        //mois a 30jour
        if (Date.jour<30)
        {
             Date.jour++;
             return Date;
        }
        else
        {
            Date.jour=1;
            Date.mois++;
            return Date;
        }
        break;

    }

}


void chargement_init(int anneedb,int anneefin)//chargement initiale de fichier TOF
{
    Buffer buf;

    TOF Genere_aleat;


    Ouvrir_TOF(&Genere_aleat,"Generertemp.bin",'N');
    if (Genere_aleat.fichier!=NULL)
    {
    char Ville[49][26];//contient les noms de wilaya
    tabvilinit(Ville);//initialiser le tableau des villes
    date Date;
    Date.annee=anneedb;
    Date.jour=1;
    Date.mois=1;
        fseek(Genere_aleat.fichier,sizeof(entete),SEEK_SET);//pour utiliser l'ecriture sequentiel a partir depremier bloc
        srand(time(NULL));

        while (Date.annee<anneefin)//tant que on est pas arrivé a la dernier date
        {
            for(int i=1;i<49;i++)//il y a 48 wilaya
            {
                buf.nbelement=0;
                for(int j=0;j<TAILLE_BUF;j++)//remplir le buffer par 3 temperatures par date
                {
                    buf.tab[j].Date=Date;
                    strcpy(buf.tab[j].nomville,Ville[i]);
                    buf.tab[j].temperature=rand()%40;//generer des temeratures entre 0 et 40
                    buf.nbelement++;//incrementer le nombre d'enregistrement dans le bloc
                }

                EcrireSeq_TOF(&Genere_aleat,buf);//une fois le buffer est plein on l'insere
        }
            Date=incremente(Date);//incrementer la date
        }
        Fermer_TOF(&Genere_aleat);//a la fin on ferme le fichier
    }


}

void tabvilinit(char Ville[49][26])//pour sauvegarder les villes  depuis le fichier ville algerie
{
    FILE *fichierdesvilles=NULL;
    fichierdesvilles=fopen("Villealgerie.txt","r");
    if(fichierdesvilles!=NULL)
    {
        rewind(fichierdesvilles);
        int i=1;
        while(i<50)
        {
            fgets(Ville[i],26,fichierdesvilles);
            i++;
        }
    }
    fclose(fichierdesvilles);
}


int rechercheindex(char nom[TAILLE_NOM_VILLE],caseindex index[49],int *trouv)//l index qui va m'aider dans la conversion
                                                                            //retourner le numero de la case ou se trouv la ville
{
    *trouv=0;
    int i;
    for (i=0;i<49;i++)
    {
        if(strcmp(nom,index[i].ville)==0)//comparer le nom de la ville a celui de l'index
        {
            *trouv=1;
            break;//on arrete la boucle si on trouv
        }
    }
    return i;
}
void creationindextof(TOF *fichier)//pour faciliter la conversion je cree un index de fichier tof creer
{
    if (fichier!=NULL)
    {
        caseindex index[50];//la table de l'index

        char ville[49][26];//la table des ville
        int cellule;//la case de l'index ou trouver la ville
        int trouv,k=1,c=0;
        Buffer buf;
        tabvilinit(ville);//initialiser la table des villes lu a partir de fichier villealgerie
        for(int i=0;i<50;i++)
        {
            strcpy(index[i].ville,ville[i+1]);//coper le nom de la ville dans l'index
            for(int j=0;j<2000;j++)
            {index[i].nbloc[j]=0;index[i].casevide=0;}//inserer les numeros de bloc ou se trouve la ville
        }
        while (k<=Entete_TOF(fichier,0))//lire tout les bloc de tof
        {
            LireDir_TOF(fichier,k,&buf);
            cellule=rechercheindex(buf.tab[0].nomville,index,&trouv);//rechercher la ville dans l'index
            if (trouv)//si on la trouv
            {
                index[cellule].nbloc[index[cellule].casevide]=k;//on insere le num de bloc dans la case vide de la cellule ou on se trouve la ville
                index[cellule].casevide++;//on incremente le num de la case vide
                k++;//bloc suivant

            }
        }
        FILE *indexfic=NULL;
        indexfic=fopen("index.bin","wb");
        if(indexfic!=NULL)
        {
            fwrite(&index,50*sizeof(caseindex),1,indexfic);//enregistrer l'index dans un fichier
        }
    }
}





int str_istr (const char *cs, const char *ct)
//recherche l'index de la sous chaine ct je l'ai copier sur un site developpez.com
{
   int index = -1;

   if (cs != NULL && ct != NULL)
   {
      char *ptr_pos = NULL;

      ptr_pos = strstr (cs, ct);
      if (ptr_pos != NULL)
      {
         index = ptr_pos - cs;
      }
   }
   return index;
}


/**********AFFICHAGE DE MENU PRINCIPALE ***************/

void menu()
{
    system("cls");
    printf("---------------------- TP SFSD -------------------\n");
    printf("--------------------------------------------------\n");
    printf("--------------Rezak AZIZ/Nassim LAGHOUB------------\n \n\n\n");
    printf("1- Chargement initiale de fichier TOF\n");//on affiche l'entete une fois créer
    printf("2- Conversion de TOF en TOVC\n");//on affiche l'entete une fois crée
    printf("3- Insertion d'une nouvelle temperature\n");
    printf("4- Insertion 144 temperature \n");
    printf("5- Requete a intervalle \n");
    printf("\n\n                             0-Quitter\n");
    printf("    choix:  ");

}
