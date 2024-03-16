#JAIME HERNÁNDEZ DELGADO  48776654W

set terminal png
set xrange [5:30]
set yrange [0:50]
set output "pow2.png"

set xlabel "n"
set ylabel "Program steps"

plot "pow2.steps" using 1:2 with lines title "pow2_1(n) = \Theta(n)", \
     "pow2.steps" using 1:3 with lines title "pow2_2(n) = \Theta(log n)", \
     "pow2.steps" using 1:4 with lines title "pow2_3(n) = \Theta (2^n)"
