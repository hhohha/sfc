OPT=-O3

project: demo_digits demo_visual demo_visual_clusters

demo_visual: demo_visual.cpp som.o
	g++ ${OPT} -Wall -W -pedantic demo_visual.cpp som.o -o demo_visual

demo_visual_clusters: demo_visual_clusters.cpp som.o
	g++ ${OPT} -Wall -W -pedantic demo_visual_clusters.cpp som.o -o demo_visual_clusters

demo_digits: demo_digits.cpp som.o
	g++ ${OPT} -Wall -W -pedantic demo_digits.cpp som.o -o demo_digits

som.o: som.cpp som.h
	g++ ${OPT} -Wall -W -pedantic -c som.cpp

clean:
	rm -f demo_visual demo_visual_clusters demo_digits *.o
