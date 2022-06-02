# @file Makefile
# @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
# @date 29/08/2021
# @brief 
# @copyright Copyright (c) 2022


SRC = src
INC = include
OBJ = obj
BIN = bin

NFEATURES = -D N_FEATURES=$(N_FEATURES)

all: build_mpi

pruebas: build_p run_p

run:
	./pso

run_mpi: 
	mpirun --bind-to none --map-by node --host localhost ./pso

build_knn:
	g++ -o knn -Iinclude $(SRC)/main_knn.cpp $(SRC)/lector.cpp $(SRC)/knn.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_knn_o2:
	g++ -O2 -o knn -Iinclude $(SRC)/main_knn.cpp $(SRC)/lector.cpp $(SRC)/knn.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build:
	g++ -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_o2:
	g++ -O2 -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_mpi:
	mpic++ -O2 -o pso -Iinclude $(NFEATURES) $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

run_p:
	./pruebas -nP 1 -nH 2 -nI 3 -k 4 

build_p:
	g++ -O2 -o prueba -Iinclude $(SRC)/pruebas.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_p_mpi:
	mpic++ -O2 -o prueba -Iinclude $(SRC)/pruebas.cpp 

clean:
	rm pso
	rm pruebas
	

