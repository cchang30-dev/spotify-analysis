set terminal png size 1000,600
set output 'correlation_with_popularity.png'
set title 'Correlation of Audio Features with Popularity'
set xlabel 'Feature'
set ylabel 'Correlation'
set style data histograms
set style fill solid
set xtics rotate by -30
set yrange [-0.15:0.1]
plot 'correlation_with_popularity.dat' using 2:xtic(1) title 'Correlation'
