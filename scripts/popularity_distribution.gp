set terminal png size 900,600
set output 'popularity_distribution.png'
set title 'Distribution of Popularity Scores'
set xlabel 'Popularity'
set ylabel 'Number of Tracks'
set boxwidth 0.8
set style fill solid
plot 'popularity_distribution.dat' using 1:2 with boxes title 'Tracks'
