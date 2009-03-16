set terminal postscript eps font "NimbusSanL-Regu"
set output "benchmark.eps"

set encoding iso_8859_15

set size 1.5, 1

set key top right spacing 2

set xlabel "taille"
set ylabel "temps (secondes)"

set tic scale 0

set xtics ("2048"  1, "1024"  2, "512"  3, "256"  4, "128" 5, "64" 6)

set logscale x 2

set xrange [1:6] reverse
set yrange [0:1800]

# grille
set style line 1 lt 2 lw 1 lc rgb "#888a85"

# ligne rouge
set style line 2 lt 1 lw 5 lc rgb "#e70f00"

# ligne verte
set style line 3 lt 1 lw 5 lc rgb "#48b13c"

set grid ls 1

plot \
"benchmark.dat" using 2 title "OPENMP=1" with lines ls 3, \
"benchmark.dat" using 1 title "OPENMP=0" with lines ls 2

