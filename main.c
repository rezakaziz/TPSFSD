#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Machine_abs_TOF.h"
int main()
{
    TOF test;

    Ouvrir_TOF(&test,"test.txt",'A');
    Buffer buf1,buf2;
    buf1.nbelement=1;
    buf1.tab[0].Date.jour=(char)10;
    buf1.tab[0].Date.mois=(char)06;
    buf1.tab[0].Date.annee=1999;
    strcpy(buf1.tab[0].nomville,"Tigzirt");
    buf1.tab[0].temperature=12;
    EcrireDir_TOF(&test,1,buf1);
    LireDir_TOF(&test,1,&buf2);
    printf("%d\n\n",buf2.tab[0].Date.annee);
    Fermer_TOF(&test);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
