set terminal png size 1400,700
set output 'avg_popularity_by_genre.png'
set title 'Average Popularity by Genre'
set xlabel 'Genre'
set ylabel 'Average Popularity'
set style data histograms
set style fill solid
set xtics rotate by -60
set xtics font ',8'
set yrange [0:70]
plot 'avg_popularity_by_genre.dat' using 2:xtic(1) title 'Average Popularity'
