SRC = src
INC = include
OBJ = obj
BIN = bin

all: build run

run:
	./pso

build:
	g++ -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp -fopenmp

clean:
	

