#ifndef DONNEES_H
#define DONNEES_H

typedef enum {
    DONNEES_ALEATOIRES,
    DONNEES_CROISSANTES,
    DONNEES_DECROISSANTES,
    DONNEES_DOUBLONS,
    NB_TYPES
} TypeDonnees;

void graine(unsigned long long s);                 /* rend les tests reproductibles */
void generer(int *t, int n, TypeDonnees type);
const char *nom_type(TypeDonnees type);            /* nom utilisé dans le CSV et Gnuplot */

#endif
