compile:
	g++ -Wall -W -pedantic -o som som.cpp
	
run:
	./som
	
all: compile run
