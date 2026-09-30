#include "donnees.h"

/* Petit générateur xorshift64 : identique sur tous les systèmes
   (rand() ne renvoie que 0..32767 sous Windows). */
static unsigned long long etat = 88172645463325252ULL;

void graine(unsigned long long s) { etat = s ? s : 1; }

static unsigned long long alea(void) {
    etat ^= etat << 13;
    etat ^= etat >> 7;
    etat ^= etat << 17;
    return etat;
}

void generer(int *t, int n, TypeDonnees type) {
    for (int i = 0; i < n; i++) {
        switch (type) {
            case DONNEES_ALEATOIRES:     t[i] = (int)(alea() % 1000000000ULL); break;
            case DONNEES_CROISSANTES:    t[i] = i; break;
            case DONNEES_DECROISSANTES:  t[i] = n - i; break;
            case DONNEES_DOUBLONS:       t[i] = (int)(alea() % (unsigned long long)(n / 100 + 1)); break;
            default: t[i] = 0;
        }
    }
}

const char *nom_type(TypeDonnees type) {
    switch (type) {
        case DONNEES_ALEATOIRES:    return "aleatoire";
        case DONNEES_CROISSANTES:   return "croissant";
        case DONNEES_DECROISSANTES: return "decroissant";
        case DONNEES_DOUBLONS:      return "doublons";
        default:                    return "inconnu";
    }
}
