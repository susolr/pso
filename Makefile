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

CXXFLAGS += -I/usr/include/websocketpp -I/usr/include/boost -I/usr/include/nlohmann
LDFLAGS  += -lboost_system -lpthread


all: build_mpi

pruebas: build_p_mpi run_p

knn: build_knn_o2

run:
	./pso

run_mpi: 
	mpirun --bind-to none --map-by node --host localhost ./pso

build_knn:
	g++ -o knn -Iinclude $(SRC)/main_knn.cpp $(SRC)/lector.cpp $(SRC)/knn.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_knn_o2:
	g++ -O2 -o knn -Iinclude $(SRC)/main_knn.cpp $(SRC)/lector.cpp $(SRC)/knn.cpp $(SRC)/parametro.cpp $(SRC)/tinyxml2.cpp -fopenmp

build:
	g++ -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_o2:
	g++ -O2 -o pso -Iinclude $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_mpi:
	mpic++ -O2 -std=c++17 -o pso -Iinclude $(CXXFLAGS) $(LDFLAGS) $(NFEATURES) $(SRC)/main.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp $(SRC)/webSocketServer.cpp $(SRC)/pso_manager.cpp -fopenmp

run_p:
	./pruebas -nP 1 -nH 2 -nI 3 -k 4 

build_p:
	g++ -O2 -o prueba -Iinclude $(SRC)/pruebas.cpp $(SRC)/lector.cpp $(SRC)/pso.cpp $(SRC)/particula.cpp $(SRC)/parametro.cpp $(SRC)/paramlist.cpp $(SRC)/tinyxml2.cpp -fopenmp

build_p_mpi:
	mpic++ -O2 -o prueba -Iinclude $(SRC)/pruebas.cpp 

build_hybrid_mpi:
	mpic++ -O2 -o hybrid -Iinclude $(SRC)/hybrid.cpp -fopenmp

clean:
	rm pso
	rm pruebas
	

