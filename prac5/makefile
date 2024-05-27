#JAIME HERNÁNDEZ DELGADO 48776654W

OPTS = -Wall -O3 -std=c++0x

all: pow2 pow2.steps graphs

pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc 

pow2.steps: pow2
	./pow2 > pow2.steps

graphs: pow2.steps pow2.gpi
	gnuplot pow2.gpi

tar: pow2.cc pow2.gpi makefile
	tar cvfz 48776654W.tar.gz pow2.cc pow2.gpi makefile
clean: 
	-rm pow2 pow2.steps fit.log *.png 
 
