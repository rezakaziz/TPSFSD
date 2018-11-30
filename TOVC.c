#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Machine_abs_TOF.h"
#include "Machine_abs_TOVC.h"
#include "constante.h"
#include "Fonction.h"

/*Structure de données: #ville$jour-mois-annee+temp1+temp2+temp3+......$date2.......*/



void tbloc_tof_vers_chaine(Buffer buf,char chaine[26])//ecrire bufftof sous la forme jour-mois-annee+temp1+temp2+temp3$
{
    strcpy(chaine,"");
    sprintf(chaine,"%d-%d-%d+%d+%d+%d$",buf.tab[0].Date.jour,buf.tab[0].Date.mois,buf.tab[0].Date.annee,\
                                        buf.tab[0].temperature,buf.tab[1].temperature,buf.tab[2].temperature);

}
void extrairechaine(char chaine[taibuffer],int start,int endd,char chaine2[taibuffer])//extraire une sous chaine de chaine entre les indice start et endd
{
    strcpy(chaine2,"");
    for(int i=start;i<endd;i++)
    {
        chaine2[i-start]=chaine[i];//mettre la lettre (i) de la chaine dans la (i-start) de la chaine chaine
    }
    chaine2[endd]='\0';
}
void ecrirebuftof_danstovc(Buffer buftof,Buffertovc *buftovc,char reste[26])//ecrire le buffer de tof dans le buffer de tovc et reccuperer le reste
{
    char chaine[26]="";
    tbloc_tof_vers_chaine(buftof,chaine);//ecrire bufftof sous la forme jour-mois-annee+temp1+temp2+temp3$
    if((1023-(strlen(buftovc->tab)))>strlen(chaine))//si il ya suffisemment d espace dans le buftovc on insere directement
    {
        strcat(buftovc->tab,chaine);

    }
    else//sinon
    {
        char chaine2[26]="";
        int start=0;
        int endd=1023-(strlen(buftovc->tab));//on recupere l'espace qui reste dans tovc
        extrairechaine(chaine,start,endd,chaine2);//extraire une chaine de cette taille depuis buftof
        strcat(buftovc->tab,chaine2);//remplir le buftovc

        extrairechaine(chaine,endd,26,chaine2);//extraire le reste
        strcpy(reste,chaine2);//recuperer le reste pour le retourner
    }
}
void insertnomville(char ville[26],Buffertovc *buftovc,char reste[26])//inserer le nom de la ville et recuperer le reste si debordement
{
    if((1023-(strlen(buftovc->tab)))>strlen(ville)+2)//si il ya suffisemment d'espace
    {
        strcat(buftovc->tab,"#");//anoncer le debut d'un autre enregistrement
        strcat(buftovc->tab,ville);//copier le nom de la ville
        strcat(buftovc->tab,"$");//fin de champ ville


    }
    else
    {
        char chaine2[26]="";
        int start=0;//start c le debut de la chaine a extraire
        int endd=1023-(strlen(buftovc->tab));//pour lafin de la chaine a extraire
        extrairechaine(ville,start,endd,chaine2);//extraire de nom de la ville une chainde la taille restante dans le buffer tovc
        strcat(buftovc->tab,chaine2);//concatiner le chaine extraite dans le buffer tovc

        extrairechaine(ville,endd,26,chaine2);//extraine le reste de nom de la ville
        strcpy(reste,chaine2);//le mettre dans la variable reste pour le reccuperer
    }
}

void conversionTOFversTOVC(TOF *f,TOVC *f2)//convertir le fichier tof en tovc
{
    caseindex cindex[50];//il va contenir la table de l'index de l'index
    FILE *index=NULL;
    index=fopen("index.bin","rb+");//lire l'index de tof
    if (index!=NULL)
    {
        fread(&cindex,50*sizeof(caseindex),1,index);//lire depuis le fichier index
    }
    else exit(EXIT_FAILURE);//index non trouvé
    Buffertovc buftovc;
    Buffer buftof;

    char reste[26]="";//au cas ou le buffer est plein on recupere le reste
    strcpy(buftovc.tab,"");
    int k=1;//c le numero de buffer
    int i=0;//c le numero de la wilaya
    aff_Entete_TOVC(f2,1,0);//initialiser l'entete a 0
    while(i<48)
    {

        insertnomville(cindex[i].ville,&buftovc,reste);//inserer le nom de la ville lu a partir de l index
        if (strlen(reste)!=0)//reste=0 si il reste suffisemment d'espace dans le buffer tovc
        {
            EcrireDir_TOVC(f2,k,buftovc);//ecrire le buffer dans le fichier si il y a un reste
            k++;//incrementer le num de buffer puisque on a ecrit (enfaite je pouvais faire une ecriture sequentiel mais pour etre
                                                                   //plus sur de numero de buffer j'ai utilisé ecriredir
            strcpy(buftovc.tab,"");
            strcpy(buftovc.tab,reste);//copier le reste dans le buffer

            strcpy(reste,"");//reinitialiser le reste a la chaine vide

        }
        for(int j=0;j<cindex[i].casevide;j++)//l'utilité de case index apparait dns l'acceleration de la conversion
        {                                    // parceque la table contient le nom de la ville et tt les buffer dont elle apparait dans tof
            LireDir_TOF(f,cindex[i].nbloc[j],&buftof);//lire les enregistrement de tof

            ecrirebuftof_danstovc(buftof,&buftovc,reste);//ecrire dans le buffer de tovc et retourner le reste si le buffer est plein
             if (strlen(reste)!=0)//reste =0 si il y a suffisemment d'espace dans le buuffer
            {
                EcrireDir_TOVC(f2,k,buftovc);//si il y a un reste ecrire le buffer de tovc
                k++;
                strcpy(buftovc.tab,"");
                strcpy(buftovc.tab,reste);//ecrire le reste dnas le buffer pour lui concatiner d 'autre enregistement

                strcpy(reste,"");

            }

        }

        i++;
    }
    strcat(buftovc.tab,"#");//pour indiquer fin des enregistement .. si je le mets pas j'aurais un blem quand j'insere a la dernier ville
    EcrireDir_TOVC(f2,k,buftovc);//ecrire le dernier buffer
}

void ecrirechaine_danstovc(char chaine22[taibuffer],Buffertovc *buftovc,char reste[taibuffer])//remplir le buffer tovc avec une chaine et retourner le reste
{
    char chaine[taibuffer];
    strcpy(chaine,chaine22);
    if((1023-strlen(buftovc->tab))>strlen(chaine))//ecrire directement s'il y a de vide
    {
        strcat(buftovc->tab,chaine);
        strcpy(reste,"");

    }
    else
    {
        char chaine2[taibuffer]="";
        int start=0;
        int endd=1023-strlen(buftovc->tab);
        extrairechaine(chaine22,start,endd,chaine2);//extraire une chaine entre start et endd et la recperrer dans chaine2
        strcat(buftovc->tab,chaine2);//la mettre dans le buffer

        extrairechaine(chaine,endd,taibuffer,chaine2);
        strcpy(reste,chaine2);
    }
}


void recherchetovc(char ville[26],int *i,int *j,int *debut, int *fin,int *trouv)//rechercher la ville dnas tovc
                                                                            //i,j bloque debut fin pour les positiondans les bloc
                                                                            //i bloc debut
                                                                            //j bloc fin
                                                                            //debut position debut dans le bloc i
                                                                            //fin position fin dans le bloc j
{
    TOVC fic;
    Buffertovc buf,buf2;
    ouvrir_TOVC(&fic,"resu_conv.bin",'a');
    (*trouv)=0;(*i)=1;(*j)=0;//initialiser les variables
    int dernierbloc=Entete_TOVC(&fic,0);
    while((((*i)<dernierbloc+1)) &&(!(*trouv) ))//on s arrete si on trouv la ville ou si on arrive au dernier bloc
    {
    LireDir_TOVC(&fic,*i,&buf);//lecture sequentiel
    cptlec++;//compter le nombre de lecture
    if (str_istr(buf.tab,"#")>-1)//si on detecte le debut d'une ville
    {

        char chaine[26],chaine2[26];
        *debut=str_istr(buf.tab,"#")+1;//le debut de la ville
        if (((*debut)+24)<1024)//si tout le nom de la  ville est contenu dans buffer(traiter le chevauchement)
        {
            extrairechaine(buf.tab,*debut,*debut+24,chaine2);//extraire e debut de l'enregitrement qui contient la ville
            if(str_istr(chaine2,ville)>-1)//si y a pas de chevauchment au niveau de nom de la ville
            {*trouv=1;//trouv a 1
            strcpy(buf2.tab,"");
            *j=(*i);//initialiser j
            while((str_istr(buf2.tab,"#")==-1) && ((*j)<Entete_TOVC(&fic,0)))//chercher la fin de l'enregistement de cette ville
            {
                (*j )++;
                LireDir_TOVC(&fic,*j,&buf2);
                cptlec++;//compteur de lecture
                *fin=str_istr(buf2.tab,"#")-1;//detecter la fin
            }
            }else (*i)++;//passer ou bloc suivant
        }
        else//si le nom de la ville risque de chevauchemt
        {
            extrairechaine(buf.tab,*debut,1024,chaine2);//extraire la fin de bloc
            LireDir_TOVC(&fic,(*j)+1,&buf2);//lire le bloc suivant
            cptlec++;
            strtok(buf2.tab,"$");//pour  le concatainer avec la chaine extraite de bloc precident et avoir le nom de la ville complete
            strcat(chaine2,buf2.tab);
            if(str_istr(chaine2,ville)>-1)//si le nom de la ville existe
            {*trouv=1;
            strcpy(buf2.tab,"");
            *j=(*i);//initialiser j
            while((str_istr(buf2.tab,"#")==-1) && ((*j)<Entete_TOVC(&fic,0)))//chercher la fin de cet enregistrement
            {
                (*j )++;
                LireDir_TOVC(&fic,*j,&buf2);
                cptlec++;
                *fin=str_istr(buf2.tab,"#")-1;//trouver la fin dans le dernier bloc
            }
            }else (*i)++;//passer au bloc suivant
        }
    }
    else
        (*i)++;//passer au bloc suivant
    }
    Fermer_TOVC(&fic);
    fic.fichier=NULL;
}


void recherchedat(date DATe,int *i,int j,int *debut,int fin,int *trouv)//rechercher la date entre deux bloc
{
    TOVC fic;Buffertovc buf;char chaine[15];int l;
    ouvrir_TOVC(&fic,"resu_conv.bin",'a');
    sprintf(chaine,"%d-%d-%d",DATe.jour,DATe.mois,DATe.annee);//ecrire la date sous forme d'une chaine
    *trouv=0;
    while ((*i<=j)&&(!(*trouv)))//lire sequentielement jusqu'a trouver ou arriver au bloc finale
    {
        LireDir_TOVC(&fic,*i,&buf);cptlec++;
        l=str_istr(buf.tab,chaine);//chercher la chaine dans le buffer
        if (l>-1)//si trouv
        {
            (*trouv) =1;
            (*debut)=l;
        }
        else
    {
        //verifier l'extrimité pour traiter le cas de chevauchemnt
        char chaine2[30]="",chaine3[30]="";
        Buffertovc buf2;
        LireDir_TOVC(&fic,(*i)+1,&buf2);//lire le bloc suivant
        cptlec++;
        extrairechaine(buf.tab,1015,1023,chaine2);//extraire la 1ere extremité (bloc 1)
        extrairechaine(buf2.tab,0,9,chaine3);//extraire la 2eme extrimité (bloc 2)
        strcat(chaine2,chaine3);//concatiner les 2 chaine
        int l1;
        l1=str_istr(chaine2,chaine);//chercher la date dans la china resultat

        if(l1>-1)//si trouv
        {
            (*trouv)=1;
            (*debut)=1015+l1;//detecter le debut
        }
        else (*i)++;//passer au bloc suivant si non trouv
    }
    }
    Fermer_TOVC(&fic);
    fic.fichier=NULL;

}

void insertion(Tenreg_ville enr)//inserer un enregistrement dans tovc
{
    int i,j,d,f,trouv;
    TOVC fic;
    recherchetovc(enr.nomville,&i,&j,&d,&f,&trouv);//rechercher la ville
    if(trouv)//si elle existe
    {
        //rechercher la date si elle existe
        trouv=0;
        recherchedat(enr.Date,&i,j,&d,f,&trouv);
        if (trouv)
        {

            Buffertovc buf,buf2;
            strcpy(buf.tab,"");
            strcpy(buf2.tab,"");
            char chaine[taibuffer]="",chaine22[taibuffer]="";
            ouvrir_TOVC(&fic,"resu_conv.bin",'a');
            LireDir_TOVC(&fic,i,&buf);cptlec++;//lire le bloc ou il existe l'enregistrement

            extrairechaine(buf.tab,0,d,buf2.tab);//decouper a l'emplacement date
            extrairechaine(buf.tab,d,1024,chaine);
            int k=str_istr(chaine,"+");//chercher ou on v inseerer la temperature
           if(k>-1)//si trouv
            {
  cas1:
                extrairechaine(chaine,0,k,chaine22);//decouper la ou il faut inserer la date
                strcat(buf2.tab,chaine22);//mettre la 1ere partie dans buf2
                strcat(buf2.tab,"+");//concatiner le separateur +
                itoa(enr.temperature,chaine22,10);//convertir la temperature en chaine
                strcat(buf2.tab,chaine22);//concatiner la chaine
                extrairechaine(buf.tab,d+k,1024,chaine);//extraire le reste (2eme partie)

  cas2:
                ecrirechaine_danstovc(chaine,&buf2,buf.tab);//ecrire la chaine dans buf et trater le chevauchement

                fic.fichier=NULL;
                ouvrir_TOVC(&fic,"resu_conv.bin",'a');

                EcrireDir_TOVC(&fic,i,buf2);//ecrire le buffer

                cptecr++;
                i++;
                while(i<=Entete_TOVC(&fic,0))//faire des decalage tout au long de fichier
                {
                    strcpy(buf2.tab,buf.tab);
                    LireDir_TOVC(&fic,i,&buf);
                    cptlec++;//compteur de lectures
                    ecrirechaine_danstovc(buf.tab,&buf2,chaine);//ecrire la chaine dans buf et trater le chevauchement
                    strcpy(buf.tab,chaine);
                        strcat(buf.tab,"\0");

                    EcrireDir_TOVC(&fic,i,buf2);
                    strcpy(buf2.tab,"");
                    cptecr++;
                    i++;
                }
                if(strlen(buf.tab)!=0)
                {

                    cptecr++;
                    EcrireDir_TOVC(&fic,i,buf);
                    }//ecrire le reste s il existe

                Fermer_TOVC(&fic);
            }
            else
            {
                i++;//passer au bloc suivant
                LireDir_TOVC(&fic,i,&buf);cptlec++;//lire le bloc puis initialiser les variable avant le goto
                strcpy(buf2.tab,"");
                strcpy(chaine,buf.tab);
                k=str_istr(buf.tab,"+");
                d=0;

                goto cas1;
            }

        }
        else//si la date non trouver on l insere
        {

            Buffertovc buf,buf2;
            strcpy(buf.tab,"");
            strcpy(buf2.tab,"");
            char chaine[taibuffer]="",chaine22[taibuffer]="";
            ouvrir_TOVC(&fic,"resu_conv.bin",'a');
            LireDir_TOVC(&fic,j,&buf);cptlec++;//lire le dernier bloc de la ville
            extrairechaine(buf.tab,0,f+1,buf2.tab);//decouper pour separer l enrregistrement suivant
            extrairechaine(buf.tab,f+1,1024,chaine);

             i=j;
             sprintf(chaine22,"%d-%d-%d+%d$",enr.Date.jour,enr.Date.mois,enr.Date.annee,enr.temperature);//formatter l enregistrement
             strcat(chaine22,chaine);//concatiner la 2eme partie
             strcpy(chaine,chaine22);//copier chain22 dans chaine pou preparer le goto
             goto cas2;
            }

        }

    }



void requete(char chaine[26],char chaine2[26])
{
    char ville[49][26];
    int i,j,d,f,trouv;
    recherchetovc(chaine,&i,&j,&d,&f,&trouv);//rechercher la premiere ville
    if (trouv)//si on la trouve
    {
        int i2,j2,d2,f2,trouv2;
        recherchetovc(chaine2,&i2,&j2,&d2,&f2,&trouv2);//rechercher la 2eme ville
        if ((trouv2)&(i<i2))//si la 2eme ville existe et sa clé est superieur a la premiere ville
        {
            TOVC fic;
            Buffertovc buf,buf2;
            ouvrir_TOVC(&fic,"resu_conv.bin",'a');
            LireDir_TOVC(&fic,i,&buf);i++;
            cptlec++;
            while(i<j2)
            {
                LireDir_TOVC(&fic,i,&buf);
                cptlec++;
                i++;

            }

            LireDir_TOVC(&fic,i,&buf);
            cptlec++;

        }
    }

}
