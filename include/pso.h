/**
 * @file pso.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef PSO_H
#define PSO_H

#include <vector>
#include "particula.h"
using namespace std;

class PSO {
public:
    PSO();
    void crearCumulo();
    void ejecutar();
    void mostrarResultados();
    void valorar();
    void infoHybrid();

private:

    vector<Particula> cumulo;
    vector<int> b_pos;
    double b_value;
    double b_k;
    int cont_cambio;
    int dimension;
    Particula particula;
    void crearCumulo(int n_p);


};


#endif