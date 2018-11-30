#ifndef TOVC_H_INCLUDED
#define TOVC_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Machine_abs_TOF.h"
#include "Machine_abs_TOVC.h"
#include "constante.h"
#include "Fonction.h"

void tbloc_tof_vers_chaine(Buffer buf,char chaine[26]);
void extrairechaine(char chaine[26],int start,int endd,char chaine2[26]);
void ecrirebuftof_danstovc(Buffer buftof,Buffertovc *buftovc,char reste[26]);
void insertnomville(char ville[26],Buffertovc *buftovc,char reste[26]);
void conversionTOFversTOVC(TOF *f,TOVC *f2);
void recherchetovc(char ville[26],int *i,int *j,int *debut, int *fin,int *trouv);
void recherchedat(date DATe,int *i,int j,int *debut,int fin,int *trouv);
void insertion(Tenreg_ville enr);
#endif // TOVC_H_INCLUDED
