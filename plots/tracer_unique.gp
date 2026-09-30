# Usage (depuis la racine du projet) : gnuplot plots/tracer_unique.gp
# Génère UN SEUL graphique : plots/graphe_final.png

# ---- À modifier selon ce que le prof demande ----
type_donnees = "aleatoire"        # aleatoire | croissant | decroissant | doublons
colonne      = 4                  # 4 = temps, 5 = comparaisons, 6 = mouvements
etiquette_y  = "Temps d'exécution (s)"
# --------------------------------------------------

set terminal pngcairo size 1000,700 enhanced font "Arial,12"
set output "plots/graphe_final.png"
set datafile separator ","
set grid
set logscale xy
set format x "10^{%L}"
set key top left box
set title sprintf("Comparaison des algorithmes de tri — données %s", type_donnees)
set xlabel "Taille du tableau n"
set ylabel etiquette_y

algos = "bulles selection insertion fusion rapide tas"

plot for [a in algos] "data/resultats.csv" \
     using (strcol(1) eq a && strcol(2) eq type_donnees ? column(3) : 1/0):(column(colonne)) \
     with linespoints lw 2 pt 7 ps 0.8 title a
unset output
print "Graphique généré : plots/graphe_final.png"
