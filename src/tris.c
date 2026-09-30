#include <stdlib.h>
#include "tris.h"

Stats g_stats;

void stats_reset(void) {
    g_stats.comparaisons = 0;
    g_stats.mouvements = 0;
}

/* ---- Fonctions élémentaires qui alimentent les compteurs ---- */
static inline int inf(int a, int b) {   /* a < b ? */
    g_stats.comparaisons++;
    return a < b;
}

static inline int sup(int a, int b) {   /* a > b ? */
    g_stats.comparaisons++;
    return a > b;
}

static inline void echange(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
    g_stats.mouvements += 3;
}

/* ---- Tris quadratiques ---- */
void tri_bulles(int *t, int n) {
    for (int i = 0; i < n - 1; i++) {
        int echange_fait = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (inf(t[j + 1], t[j])) {
                echange(&t[j], &t[j + 1]);
                echange_fait = 1;
            }
        }
        if (!echange_fait) break;   /* déjà trié */
    }
}

void tri_selection(int *t, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (inf(t[j], t[min])) min = j;
        if (min != i) echange(&t[i], &t[min]);
    }
}

void tri_insertion(int *t, int n) {
    for (int i = 1; i < n; i++) {
        int cle = t[i];
        g_stats.mouvements++;
        int j = i - 1;
        while (j >= 0 && sup(t[j], cle)) {
            t[j + 1] = t[j];
            g_stats.mouvements++;
            j--;
        }
        t[j + 1] = cle;
        g_stats.mouvements++;
    }
}

/* ---- Tri fusion ---- */
static void fusionner(int *t, int *tmp, int g, int m, int d) {
    int i = g, j = m, k = g;
    while (i < m && j < d) {
        if (inf(t[j], t[i])) tmp[k++] = t[j++];
        else                 tmp[k++] = t[i++];
        g_stats.mouvements++;
    }
    while (i < m) { tmp[k++] = t[i++]; g_stats.mouvements++; }
    while (j < d) { tmp[k++] = t[j++]; g_stats.mouvements++; }
    for (k = g; k < d; k++) { t[k] = tmp[k]; g_stats.mouvements++; }
}

static void fusion_rec(int *t, int *tmp, int g, int d) {   /* intervalle [g, d[ */
    if (d - g < 2) return;
    int m = g + (d - g) / 2;
    fusion_rec(t, tmp, g, m);
    fusion_rec(t, tmp, m, d);
    fusionner(t, tmp, g, m, d);
}

void tri_fusion(int *t, int n) {
    int *tmp = malloc((size_t)n * sizeof(int));
    if (!tmp) return;
    fusion_rec(t, tmp, 0, n);
    free(tmp);
}

/* ---- Tri rapide (pivot central, on récurse sur la plus petite partie
        pour que la pile reste en O(log n)) ---- */
static void rapide_rec(int *t, int g, int d) {   /* intervalle [g, d] inclus */
    while (g < d) {
        int pivot = t[g + (d - g) / 2];
        int i = g, j = d;
        while (i <= j) {
            while (inf(t[i], pivot)) i++;
            while (inf(pivot, t[j])) j--;
            if (i <= j) {
                if (i != j) echange(&t[i], &t[j]);
                i++;
                j--;
            }
        }
        if (j - g < d - i) { rapide_rec(t, g, j); g = i; }
        else               { rapide_rec(t, i, d); d = j; }
    }
}

void tri_rapide(int *t, int n) {
    rapide_rec(t, 0, n - 1);
}

/* ---- Tri par tas ---- */
static void descendre(int *t, int i, int n) {
    for (;;) {
        int g = 2 * i + 1, d = g + 1, max = i;
        if (g < n && inf(t[max], t[g])) max = g;
        if (d < n && inf(t[max], t[d])) max = d;
        if (max == i) return;
        echange(&t[i], &t[max]);
        i = max;
    }
}

void tri_tas(int *t, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) descendre(t, i, n);
    for (int fin = n - 1; fin > 0; fin--) {
        echange(&t[0], &t[fin]);
        descendre(t, 0, fin);
    }
}
