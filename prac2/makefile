#JAIME HERNÁNDEZ DELGADO 48776654W

OPTS = -Wall -O3 -std=c++0x

all: qs-vs-hs qs-vs-hs.Msteps graphs

graphs: qs-vs-hs.Msteps qs-vs-hs.gnuplot
	gnuplot qs-vs-hs.gnuplot

qs-vs-hs: qs-vs-hs.cc
	g++ ${OPTS} -o qs-vs-hs qs-vs-hs.cc	 

qs-vs-hs.Msteps: qs-vs-hs
	./qs-vs-hs > qs-vs-hs.Msteps

tar: qs-vs-hs.cc qs-vs-hs.gnuplot makefile
	tar cvfz 48776654W.tar.gz qs-vs-hs.cc qs-vs-hs.gnuplot makefile

clean: 
	-rm qs-vs-hs qs-vs-hs.Msteps fit.log *.png 