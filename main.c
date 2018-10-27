#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Machine_abs_TOF.h"
#include "constante.h"
#include "Fonction.h"

int main()
{
    chargement_init(2000,2005);
    Buffer buf;
    TOF fichier;

    Ouvrir_TOF(&fichier,"Generertemp.bin",'A');
    if (fichier.fichier!=NULL)
    {

        printf("%d",Entete_TOF(&fichier,0));
        Fermer_TOF(&fichier);
    }

    system("pause");
    return 0;
}
