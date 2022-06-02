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

    MPI::Init_thread(MPI_THREAD_MULTIPLE);
    
    Paramlist * lista;
    if (argc==1){
        lista = Paramlist::getInstance();
    }
    else{
        lista = Paramlist::getInstance(argc, argv);
    }

    PSO mi_pso = PSO();
    double time_inicio;
    double time;
    if(stoi(lista->getValor("-rank"))==0){
        cout << "Master " << endl << flush;
        mi_pso.crearCumulo();
        time_inicio = omp_get_wtime();
        mi_pso.ejecutar();
        time = omp_get_wtime() - time_inicio;
    }
    else {
        cout << "Worker: " << MPI::COMM_WORLD.Get_size() << endl << flush;
        mi_pso.valorar();
    }

    
    //mi_pso.mostrarResultados();
    cout << time << endl;

    MPI::Finalize();


    return 0;
}