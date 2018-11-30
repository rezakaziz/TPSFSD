#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Machine_abs_TOF.h"
#include "Machine_abs_TOVC.h"
#include "constante.h"
#include "Fonction.h"
#include "TOVC.h"


int main()
{

    TOF fictof;
    TOVC fictov;
    Tenreg_ville ville;
    int choix=-1;
    clock_t deb,fi;
 debut:   cptlec=0;
   while(choix!=0)
   {
        menu();
        scanf("%d",&choix);
        switch(choix)
        {
        case 0:
            exit(EXIT_SUCCESS);//choix quitter
            break;
        case 1:
            system("cls");
            printf("---------------------- TP SFSD -------------------\n");
            printf("--------------------------------------------------\n\n\n\n");
            printf("----- Generation aleatoire de fichier TOF---------\n\n");
            int db,df;//annee debut et annee fin
            printf("Veuillez inserer l'annee de debut:");
            scanf("%d",&db);
            printf("Veuillez inserer l'annee de debut:");
            scanf("%d",&df);
            chargement_init(db,df);//chargement initiale de fichier tof sous le nom Generertemp.bin

            Ouvrir_TOF(&fictof,"Generertemp.bin",'a');
            printf("\n\nfichier cree avec l'entete:\n");//affichage de l'entete
            printf("   - Nom: Generertemp.bin\n");
            printf("   - Nombre de bloc: %d\n",Entete_TOF(&fictof,0));
            printf("   - Nombre d'enregistrement: %d\n",Entete_TOF(&fictof,1));
            Fermer_TOF(&fictof);
            system("pause");
            break;
        case 2:
            system("cls");
            printf("---------------------- TP SFSD -------------------\n");
            printf("--------------------------------------------------\n\n\n\n");
            printf("----- Conversion de TOF en TOVC---------\n\n");

            Ouvrir_TOF(&fictof,"Generertemp.bin",'a');
            printf("\n\nfichier cree avec l'entete:\n");
            printf("   - Nom: Generertemp.bin\n");
            printf("   - Nombre de bloc: %d\n",Entete_TOF(&fictof,0));
            printf("   - Nombre d'enregistrement: %d\n",Entete_TOF(&fictof,1));
            printf("\nConversion en cours\n");


            ouvrir_TOVC(&fictov,"resu_conv.bin",'N');
            creationindextof(&fictof);//creation index de tof
            conversionTOFversTOVC(&fictof,&fictov);//appel de module de conversion
            Fermer_TOVC(&fictov);
            ouvrir_TOVC(&fictov,"resu_conv.bin",'a');
            printf("fin de conversion\n ");
            printf("   - Nom: resu_conv.bin\n");
            printf("   - Nombre de bloc: %d\n",Entete_TOVC(&fictov,0));
            printf("   - taille de fichier: %d\n",Entete_TOVC(&fictov,1));
            Fermer_TOVC(&fictov);
            Fermer_TOF(&fictof);

            system("pause");
            break;
        case 3:
            system("cls");
            printf("---------------------- TP SFSD -------------------\n");
            printf("--------------------------------------------------\n\n\n\n");
            printf("----- Inserer un nouveau enregistrement---------\n\n");

            printf("Nom de ville: ");
            scanf("%s",&ville.nomville);
            printf("Jour: ");
            scanf("%d",&ville.Date.jour);
            printf("Mois: ");
            scanf("%d",&ville.Date.mois);
            printf("Annee: ");
            scanf("%d",&ville.Date.annee);
            if(!setdate(ville.Date.jour,ville.Date.mois,ville.Date.annee)){printf("date non valide\n");system("pause");goto debut;}
            printf("Temperature: ");
            scanf("%d",&ville.temperature);


            insertion(ville);
            printf("Nombre de lectures %d\n",cptlec);
            printf("Nombre d'ecritures %d\n",cptecr);
            system("pause");
            break;
        case 4:
            system("cls");
            printf("---------------------- TP SFSD -------------------\n");
            printf("--------------------------------------------------\n\n\n\n");
            printf("----- insertion de 144 temperature ---------\n\n");


            char villee[49][26];
            srand(time(NULL));

            tabvilinit(villee);//initialiser le tableau des villes pour choisir une ville aleatoirement dans cette ville
            for (int i=0;i<144;i++)
            {
                strcpy(ville.nomville,villee[1+rand()%48]);//choisir une ville aleatirement
                //strcpy(ville.nomville,"Tizi-Ouzou");
                ville.Date.jour=1+rand()%28;//choisir une journée aleatoirement
                ville.Date.mois=1+rand()%11;//choisir un mois aleatoirement
                ville.Date.annee=2000;//une annee aleatoirement entre 2000 et 2020
                ville.temperature=rand()%50;//temperature aleatoirement
                deb=clock();//initialiser le clock pour calculer le temps d execution
                insertion(ville);//inserere le ville generer
                if(cptlec==0)
                    {printf("ok");}
                fi=clock();//capturer le fin
                FILE *resultat=NULL;
                resultat=fopen("statistique.txt","r+");
                if(resultat!=NULL)
                {
                    fseek(resultat,0,SEEK_END);
                    fprintf(resultat,"%d %d %d %f\n",Entete_TOVC(&fictov,1),cptlec,cptecr,(double)fi-deb);
                    fclose(resultat);
                }
                printf("%s %d %d %d\n",ville.nomville,ville.Date.jour,ville.Date.mois,ville.Date.annee);//afficher la ville inserer
                printf("Temps d'execution: %.2f ms\n",((double)fi-deb));
                printf("Nombre de lecture %d\n",cptlec);cptlec=0;
                printf("Nombre d'ecriture %d\n",cptecr);cptecr=0;
                ouvrir_TOVC(&fictov,"resu_conv.bin",'a');
                printf("taille de fichier: %d\n\n",Entete_TOVC(&fictov,1));
                Fermer_TOVC(&fictov);
            }



            system("pause");
            break;
        case 5:
            system("cls");
            printf("---------------------- TP SFSD -------------------\n");
            printf("--------------------------------------------------\n\n\n\n");
            printf("----- Requete a intervale ---------\n\n");
            cptlec=0;

            char ville1[26],ville2[26];
            printf("Entrez la premiere ville");
            scanf("%s",ville1);
            printf("Entrez la 2eme ville");
            scanf("%s",ville2);
            deb=clock();
            requete(ville1,ville2);
            fi=clock();
            printf("Temps d'execution: %.2f ms\n",((double)fi-deb));
            printf("\n\nNombre de lecture %d\n",cptlec);
            cptlec=0;
            system("pause");
            break;
   }
   }

    system("pause");
    return 0;
}
