#include <stdio.h>
#include <stdlib.h>
#include "Machine_abs_TOF.h"
int main()
{
    TOF test;
    test.Entete.cpt_insert=50;
    Ouvrir_TOF(&test,"test.txt",'A');
    printf("%d %d",test.Entete.cpt_insert,test.Entete.nbbloc);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
