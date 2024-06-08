#JAIME HERNÁNDEZ DELGADO 48776654W

OPTS = -Wall -O3 -std=c++0x

all: quickSort quickSort.CPUtimes graphs

quickSort: quickSort.cc
	g++ ${OPTS} -o quickSort quickSort.cc	 

quickSort.CPUtimes: quickSort
	./quickSort > quickSort.CPUtimes

graphs: quickSort.CPUtimes quickSort.gpi
	gnuplot quickSort.gpi

tar: quickSort.cc quickSort.gpi makefile
	tar cvfz 48776654W.tar.gz quickSort.cc quickSort.gpi makefile

clean: 
	rm quickSort quickSort.CPUtimes fit.log quickSort*.png *~