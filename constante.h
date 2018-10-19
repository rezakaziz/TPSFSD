#ifndef CONSTANTE_H_INCLUDED
#define CONSTANTE_H_INCLUDED


// definir les constantes
#define TAILLE_NOM 26
#define TAILLE_BUF 255
#define TAILLE_NOM_VILLE 26

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


//definir la structure de bloc
typedef struct Tbloc Tbloc;
struct Tbloc
{
    Tenreg_ville tab[TAILLE_BUF];
    int nbelement;
};
//defiir le buffer comme une structure de Tbloc
typedef struct Tbloc Buffer;

typedef struct entete entete;
struct entete
{
    int nbbloc;
    int cpt_insert;
};
#endif // CONSTANTE_H_INCLUDED
