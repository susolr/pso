all: build run

run:
	./pso

build:
	g++ -o pso main.cpp lector.cpp pso.cpp particula.cpp parametro.cpp -fopenmp

clean:
	

