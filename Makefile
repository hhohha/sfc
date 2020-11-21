OPT=-O3

project: som_demo

som_demo: som_demo.cpp som.o
	g++ ${OPT} -Wall -W -pedantic som_demo.cpp som.o -o som_demo

som.o: som.cpp som.h
	g++ ${OPT} -Wall -W -pedantic -c som.cpp

clean:
	rm -f som_demo *.o
