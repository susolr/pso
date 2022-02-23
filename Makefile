# @file Makefile
# @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
# @date 29/08/2021
# @brief 
# @copyright Copyright (c) 2022


SRC = src
INC = include
OBJ = obj
BIN = bin

all: build run

pruebas: build_p run_p

run:
	./pso

build:
	g++ -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

run_p:
	./pruebas -nP 1 -nH 2 -nI 3 -k 4 

build_p:
	g++ -o pruebas -Iinclude $(SRC)/pruebas.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

clean:
	rm pso
	rm pruebas
	

