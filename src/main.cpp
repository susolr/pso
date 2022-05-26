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

    Paramlist * lista;
    if (argc==1){
        lista = Paramlist::getInstance();
    }
    else{
        lista = Paramlist::getInstance(argc, argv);
    }

    MPI::Init_thread(MPI_THREAD_MULTIPLE);

    PSO mi_pso = PSO();
    double time_inicio;
    double time;
    if(stoi(lista->getValor("-rank"))==0){
        mi_pso.crearCumulo();
        time_inicio = omp_get_wtime();
        mi_pso.ejecutar();
        time = omp_get_wtime() - time_inicio;
    }
    else {
        mi_pso.crearParticula();
        mi_pso.valorar();
    }

    
    //mi_pso.mostrarResultados();
    cout << time << endl;

    MPI_Finalize();


    return 0;
}