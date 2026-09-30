#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/tris.h"
#include "../include/donnees.h"

#define NB_REPETITIONS 3

typedef struct {
    const char *nom;      /* doit correspondre au nom utilisé dans le script Gnuplot */
    TriFn fonction;
    int taille_max;       /* 0 = pas de limite ; sinon on saute les tailles plus grandes */
} Algo;

static const Algo ALGOS[] = {
    {"bulles",    tri_bulles,    50000},
    {"selection", tri_selection, 50000},
    {"insertion", tri_insertion, 50000},
    {"fusion",    tri_fusion,    0},
    {"rapide",    tri_rapide,    0},
    {"tas",       tri_tas,       0},
};
#define NB_ALGOS ((int)(sizeof(ALGOS) / sizeof(ALGOS[0])))

static const int TAILLES[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
#define NB_TAILLES ((int)(sizeof(TAILLES) / sizeof(TAILLES[0])))

static int est_trie(const int *t, int n) {
    for (int i = 1; i < n; i++)
        if (t[i - 1] > t[i]) return 0;
    return 1;
}

/* Copie la source, lance le tri, mesure temps + compteurs. Renvoie 1 si le résultat est bien trié. */
static int executer(const Algo *a, const int *source, int *copie, int n,
                    double *temps, Stats *stats) {
    memcpy(copie, source, (size_t)n * sizeof(int));
    stats_reset();

    clock_t debut = clock();
    a->fonction(copie, n);
    clock_t fin = clock();

    *temps = (double)(fin - debut) / CLOCKS_PER_SEC;
    *stats = g_stats;
    return est_trie(copie, n);
}

/* Usage : ./benchmark [fichier_csv] [taille_max]
   ex.   : ./benchmark data/resultats.csv 100000   (test rapide) */
int main(int argc, char **argv) {
    const char *chemin = (argc > 1) ? argv[1] : "data/resultats.csv";
    int cap = (argc > 2) ? atoi(argv[2]) : 0;

    FILE *f = fopen(chemin, "w");
    if (!f) { perror(chemin); return 1; }
    fprintf(f, "algo,type,taille,temps_s,comparaisons,mouvements\n");

    int nmax = 0;
    for (int i = 0; i < NB_TAILLES; i++)
        if (TAILLES[i] > nmax && (!cap || TAILLES[i] <= cap)) nmax = TAILLES[i];

    int *source = malloc((size_t)nmax * sizeof(int));
    int *copie  = malloc((size_t)nmax * sizeof(int));
    if (!source || !copie) { fprintf(stderr, "Mémoire insuffisante\n"); return 1; }

    for (int ty = 0; ty < NB_TYPES; ty++) {
        for (int ti = 0; ti < NB_TAILLES; ti++) {
            int n = TAILLES[ti];
            if (cap && n > cap) continue;

            for (int ai = 0; ai < NB_ALGOS; ai++) {
                const Algo *a = &ALGOS[ai];
                if (a->taille_max && n > a->taille_max) continue;

                double t_tot = 0;
                unsigned long long c_tot = 0, m_tot = 0;

                for (int r = 0; r < NB_REPETITIONS; r++) {
                    graine(1000 + r);                    /* mêmes données pour tous les algos */
                    generer(source, n, (TypeDonnees)ty);

                    double t; Stats s;
                    if (!executer(a, source, copie, n, &t, &s)) {
                        fprintf(stderr, "ERREUR : %s n'a pas trié correctement (n=%d)\n", a->nom, n);
                        return 1;
                    }
                    t_tot += t; c_tot += s.comparaisons; m_tot += s.mouvements;
                }

                fprintf(f, "%s,%s,%d,%.6f,%llu,%llu\n", a->nom, nom_type((TypeDonnees)ty), n,
                        t_tot / NB_REPETITIONS, c_tot / NB_REPETITIONS, m_tot / NB_REPETITIONS);
                fflush(f);
                fprintf(stderr, "[%s] %-9s n=%-8d ok\n", nom_type((TypeDonnees)ty), a->nom, n);
            }
        }
    }

    free(source);
    free(copie);
    fclose(f);
    fprintf(stderr, "Résultats écrits dans %s\n", chemin);
    return 0;
}
