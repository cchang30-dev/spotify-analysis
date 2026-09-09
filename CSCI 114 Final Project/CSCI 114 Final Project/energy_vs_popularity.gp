set terminal png size 900,600
set output 'energy_vs_popularity.png'
set title 'Energy vs Popularity with Regression Line'
set xlabel 'Energy'
set ylabel 'Popularity'
set xrange [0:1]
set yrange [-5:105]
f(x) = 33.1785 + 0.093656 * x
plot 'energy_vs_popularity.dat' using 1:2 with points pointtype 7 pointsize 0.3 title 'Tracks', f(x) with lines lw 2 title 'Regression Line'
