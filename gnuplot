set terminal png size 1200,600
set output "histo_top10.png"
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.9
set grid ytics
set xtics rotate by -45
set ylabel "Valeur"
set title "Top 10 des meilleurs"
plot "histo.dat" using 2:xtic(1) \
     every ::0 \
     title "10 meilleurs" \
     where stringcolumn(3) eq "max"
set output "histo_bottom50.png"
set title "50 pires"
plot "histo.dat" using 2:xtic(1) \
     title "50 pires" \
     where stringcolumn(3) eq "min"
