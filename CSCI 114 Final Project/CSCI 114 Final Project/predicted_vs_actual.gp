set terminal png size 900,600
set output 'predicted_vs_actual.png'
set title 'Predicted vs Actual Popularity'
set xlabel 'Actual Popularity'
set ylabel 'Predicted Popularity'
set xrange [-5:105]
set yrange [25:45]
plot 'predicted_vs_actual.dat' using 1:2 with points pointtype 7 pointsize 0.3 title 'Predictions', x with lines lw 2 title 'Perfect Prediction'
