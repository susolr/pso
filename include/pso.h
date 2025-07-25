/**
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of Atom repository.
 *
 * This work has been funded by:
 *
 * University of Granada under grant number PPJIA-2023-25.\n
 * Spanish 'Ministerio de Ciencia, Innovación y Universidades' under grants number
 * PID2022-137461NB-C32 and PID2020-119478GB-I00.\n Spanish 'Ministerio de Universidades' as part of
 * the program of mobility stays for professors and researchers in foreign higher education and
 * research centers under grant number CAS22/00332.\n European Regional Development Fund (ERDF).
 *
 * @file pso.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the BPSO algorithm
 * @copyright Atom (c) 2024 University of Granada
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