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

using namespace std;

int main (int argc, char* argv[]){

/*
    PSO mi_pso = PSO();
    mi_pso.crearCumulo(20, 5);
    mi_pso.ejecutar();
    mi_pso.mostrarResultados();
*/
    Lector * lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDataTraining(200,10);
    lector->mostrarDataTraining();

    return 0;
}