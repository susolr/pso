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
using namespace std;

int main (int argc, char* argv[]){

    Paramlist * lista;
    if (argc==1){
        lista = Paramlist::getInstance();
    }
    else{
        lista = Paramlist::getInstance(argc, argv);
    }
    PSO mi_pso = PSO();
    mi_pso.crearCumulo();
    //medir tiempo openmp
    double time_inicio = omp_get_wtime();
    mi_pso.ejecutar();
    double time = omp_get_wtime() - time_inicio;
    //mi_pso.mostrarResultados();
    cout << "Tiempo ejecucion " << time*1000 << endl;


    return 0;
}