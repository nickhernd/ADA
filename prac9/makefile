#JAIME HERNÁNDEZ DELGADO 48776654W

OPTS = -Wall -O3 -std=c++0x -fopenmp

mcp: mcp_bb.cc
	g++ ${OPTS} -o mcp_bb mcp_bb.cc

tar: mcp_bb.cc makefile
	tar cvfz 48776654W.tar.gz mcp_bb.cc makefile memoria.pdf
