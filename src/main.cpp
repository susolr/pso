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
    /*cout << "-nH " << lista->getInstance()->getValor("-nH") << endl;
    cout << "-nP " << lista->getInstance()->getValor("-nP") << endl;
    cout << "-sI " << lista->getInstance()->getValor("-sI") << endl;
    cout << "-nI " << lista->getInstance()->getValor("-nI") << endl;
    cout << "-cS " << lista->getInstance()->getValor("-cS") << endl;
    cout << "-cC " << lista->getInstance()->getValor("-cC") << endl;
    cout << "-cI " << lista->getInstance()->getValor("-cI") << endl;
    cout << "-k " << lista->getInstance()->getValor("-k") << endl;
    */  
    //medir tiempo openmp
    double time_inicio = omp_get_wtime();
    mi_pso.ejecutar();
    double time = omp_get_wtime() - time_inicio;
    //mi_pso.mostrarResultados();
    cout << "Tiempo ejecucion " << time*1000 << endl;


    return 0;
}