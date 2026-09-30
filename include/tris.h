#ifndef TRIS_H
#define TRIS_H

/* Compteurs globaux : remis à zéro avant chaque tri */
typedef struct {
    unsigned long long comparaisons;
    unsigned long long mouvements;   /* une affectation t[i] = ... = 1 mouvement, un échange = 3 */
} Stats;

extern Stats g_stats;
void stats_reset(void);

/* Signature commune à tous les tris : permet de les mettre dans un tableau */
typedef void (*TriFn)(int *t, int n);

void tri_bulles(int *t, int n);
void tri_selection(int *t, int n);
void tri_insertion(int *t, int n);
void tri_fusion(int *t, int n);
void tri_rapide(int *t, int n);
void tri_tas(int *t, int n);

#endif
