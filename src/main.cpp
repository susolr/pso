/**
 * @file main.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"
#include "pso.h"
#include "omp.h"
#include "paramlist.h"
#include <mpi.h>

using namespace std;

int main (int argc, char* argv[]){

    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

    printf("Hybrid: Hello from process %d/%d on %s\n", rank, numprocs, processor_name);

    //MPI::Init_thread(MPI_THREAD_MULTIPLE);

    
    
    Paramlist * lista;
    if (argc==1){
        lista = Paramlist::getInstance();
    }
    else{
        lista = Paramlist::getInstance(argc, argv);
    }

    //cout << "N hebras" << lista->getValor("-nH") << endl;

    srand((uint) time(NULL) + stoi(lista->getValor("MPIrank")));

    PSO mi_pso = PSO();
    double time_inicio;
    double time;
    if(stoi(lista->getValor("MPIrank"))==0){
        //cout << "Master tamanio: " << lista->getValor("MPIsize") << endl << flush;
        mi_pso.crearCumulo();
        time_inicio = omp_get_wtime();
        mi_pso.ejecutar();
        time = omp_get_wtime() - time_inicio;
    }
    else {
        //cout << "Worker: " << lista->getValor("MPIrank") << endl << flush;
        mi_pso.valorar();
    }

    
    //mi_pso.mostrarResultados();
    cout << time << endl;

    MPI::Finalize();

    return 0;
}