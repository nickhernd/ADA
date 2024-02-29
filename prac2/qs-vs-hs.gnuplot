#JAIME HERNÁNDEZ DELGADO 48776654W

#==============================================================================

#QUICKSORT
set terminal png
set output "quickSort.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort"

plot "qs-vs-hs.Msteps" using 1:2 with lines title "random arrays" ,\
"qs-vs-hs.Msteps" using 1:4 with lines title "sorted arrays" ,\
"qs-vs-hs.Msteps" using 1:6 with lines title "reverse-sorted arrays" 

#==============================================================================

#HeapSort
set terminal png
set output "heapSort.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Heapsort"

plot "qs-vs-hs.Msteps" using 1:3 with lines title "random arrays" ,\
"qs-vs-hs.Msteps" using 1:5 with lines title "sorted arrays" ,\
"qs-vs-hs.Msteps" using 1:7 with lines title "reverse-sorted arrays" 

#==============================================================================

#qs-vs-hs-RA.png
set terminal png
set output "qs-vs-hs-RA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (random arrays)"

ya(x)=a*x*log(x) + b 
fit ya(x) "qs-vs-hs.Msteps" using 1:3 via a,b
zb(x)=c*x*log(x) + d 
fit zb(x) "qs-vs-hs.Msteps" using 1:2 via c,d

plot "qs-vs-hs.Msteps" using 1:2 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:3 with lines title "HeapSort" ,\
ya(x) title "fitting QuickSort time value to..." ,\
zb(x) title "fitting HeapSort time value to..."

#==============================================================================

#qs-vs-hs-SA.png
set terminal png
set output "qs-vs-hs-SA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (sorted arrays)"

yc(x)=e*x*log(x) + f 
fit yc(x) "qs-vs-hs.Msteps" using 1:4 via e,f
zd(x)=g*x*log(x) + h 
fit zd(x) "qs-vs-hs.Msteps" using 1:5 via g,h

plot "qs-vs-hs.Msteps" using 1:4 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:5 with lines title "HeapSort" ,\
yc(x) title "fitting QuickSort time value to..." ,\
zd(x) title "fitting HeapSort time value to..."

#==============================================================================

#qs-vs-hs-RSA.png
set terminal png
set output "qs-vs-hs-RSA.png" 

set xlabel "size of arrays"
set ylabel "Millions of program steps"

set title  "Quicksort versus Heapsort (reverse-sorted arrays)"
 
ye(x)=i*x*log(x) + j 
fit ye(x) "qs-vs-hs.Msteps" using 1:6 via i,j
zf(x)=k*x*log(x) + l 
fit zf(x) "qs-vs-hs.Msteps" using 1:7 via k,l

plot "qs-vs-hs.Msteps" using 1:6 with lines title "QuickSort" ,\
"qs-vs-hs.Msteps" using 1:7 with lines title "HeapSort" ,\
ye(x) title "fitting QuickSort time value to..." ,\
zf(x) title "fitting HeapSort time value to..."
