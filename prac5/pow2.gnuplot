#JAIME HERNÁNDEZ DELGADO  48776654W

set terminal png
set xrange [1:32]
set yrange [0:50]
set output "pow2.png"

set xlabel "n"
set ylabel "Program steps"

plot "pow2.steps" using 1:2 with lines title "pow2_1(n) = 2 * pow2_1(n-1)", \
     "pow2.steps" using 1:3 with lines title "pow2_2(n)", \
     "pow2.steps" using 1:4 with lines title "pow2_3(n)"
