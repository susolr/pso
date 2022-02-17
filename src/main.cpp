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

using namespace std;

int main (int argc, char* argv[]){


    PSO mi_pso = PSO();
    mi_pso.crearCumulo(40, 3600);
    //medir tiempo openmp
    double time_inicio = omp_get_wtime();
    mi_pso.ejecutar();
    double time = omp_get_wtime() - time_inicio;
    mi_pso.mostrarResultados();
    cout << "Tiempo ejecucion " << time*1000 << endl;

/*    Lector * lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDataTraining(200,10);
    lector->mostrarDataTraining();
*/

    return 0;
}