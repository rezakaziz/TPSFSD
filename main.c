#include <stdio.h>
#include <stdlib.h>
#include "Machine_abs_TOF.h"
int main()
{
    date DATE;
    scanf("%[^-]d-%[^-]d-%d",&DATE.jour,&DATE.mois,&DATE.annee);
    printf("%d %d %d ",DATE.jour,DATE.mois,DATE.annee);

    printf("Hello world!\n");
    system("pause");
    return 0;
}
