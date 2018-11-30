#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED


// definir les constantes
#define TAILLE_NOM 26
#define TAILLE_BUF 3
#define TAILLE_NOM_VILLE 26
#define taibuffer 1025
int cptlec;//compteur de lectures
int cptecr;//compteur d'ecritures
// definir le format de la date
typedef struct date date;
struct date
{
    int jour;
    int mois;
    int annee;
};
//definir la structure de l'enregistrement
typedef struct Tenreg_ville
{
    date Date;
    char nomville[TAILLE_NOM_VILLE];
    int temperature;
}Tenreg_ville;


//definir la structure de bloc pour TOF
typedef struct Tbloc Tbloc;
struct Tbloc
{
    Tenreg_ville tab[TAILLE_BUF];
    int nbelement;
};
//defiir le buffer comme une structure de Tbloc
typedef struct Tbloc Buffer;

//definir l'entete de tof
typedef struct entete entete;
struct entete
{
    int nbbloc;
    int cpt_insert;
};
//definir un type tof
typedef struct TOF TOF;
struct TOF
{
    FILE *fichier;
    entete Entete;
};

//************ Constante pour TOVC*******/
//entete tovc
typedef struct entetetovc entetetovc;
struct entetetovc
{
    int ind_dernier_bloc;
    int tailledefichier;


};
//type tovc
typedef struct TOVC TOVC;
struct TOVC
{
    FILE *fichier;
    entetetovc Entete;

};
//type bloc tovc
typedef struct Tbloctovc Tbloctovc;
struct Tbloctovc
{
    char tab[1024];

};
typedef struct Tbloctovc Buffertovc ;

#endif // CONSTANTE_H_INCLUDED
