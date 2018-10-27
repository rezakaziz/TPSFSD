#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Machine_abs_TOF.h"
#include "constante.h"

int anneebissextile(int annee)
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

int setdate(int day,int month,int year)
{
    if (month>0 && month<13)
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
        if (day<1 || day >31)
        {
            return 0;
        }
        break;
    case 2:
        if (anneebissextile(year))
        {
            if (day<1 || day >29)
            {
                return 0;
            }

        }
        else
        {
            if (day<1 || day>28)
            {
               return 0;
            }
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day<1 || day >30)
        {
            return 0;
        }
        break;
    default:
        return 1;


    }
    }
    else return 0;
    return 1;
}

date incremente(date Date)
{
    switch(Date.mois)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
        if (Date.jour<31)
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

    case 2:
        if (anneebissextile(Date.annee))
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
        else
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


void chargement_init(int anneedb,int anneefin)
{
    Buffer buf;

    TOF Genere_aleat;


    Ouvrir_TOF(&Genere_aleat,"Generertemp.bin",'N');
    if (Genere_aleat.fichier!=NULL)
    {
        char Ville[48][26];
        tabvilinit(Ville);
date Date;
    Date.annee=anneedb;
    Date.jour=1;
    Date.mois=1;
        fseek(Genere_aleat.fichier,sizeof(entete),SEEK_SET);
        srand(time(NULL));

        while (Date.annee<anneefin)
        {
            for(int i=1;i<49;i++)
            {
                buf.nbelement=0;
                for(int j=0;j<TAILLE_BUF;j++)
                {
                    buf.tab[j].Date=Date;
                    strcpy(buf.tab[j].nomville,Ville[i]);
                    buf.tab[j].temperature=rand()%40;
                    buf.nbelement++;
                }

                EcrireSeq_TOF(&Genere_aleat,buf);
        }
            Date=incremente(Date);
        }
        Fermer_TOF(&Genere_aleat);
    }


}

void tabvilinit(char Ville[48][26])
{
    FILE *fichierdesvilles=NULL;
    fichierdesvilles=fopen("Villealgerie.txt","r");
    if(fichierdesvilles!=NULL)
    {
        rewind(fichierdesvilles);
        int i=1;
        while(i<49)
        {
            fgets(Ville[i],26,fichierdesvilles);
            i++;
        }
    }
    fclose(fichierdesvilles);
}
