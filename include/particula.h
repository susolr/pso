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
 * @file particula.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the Particula object
 * @copyright Atom (c) 2024 University of Granada
 */

#ifndef PARTICULA_H
#define PARTICULA_H

#include <vector>

#include "lector.h"
using namespace std;

typedef struct particula_struct {
    int n_particula;
    double valor;
    int pos[N_FEATURES];
} particula_mpi;

class Particula {
   public:
    Particula();
    Particula(int n);
    void actualizarPosicion();
    void valorar();
    void actualizarVelocidad(vector<int> &g);
    double getValue();
    double getBValue();
    void setPos(vector<int> npos);
    void setValue(double val);
    vector<int> getBPos();
    vector<int> getPos();
    particula_mpi toStruct();
    void fromStruct(particula_mpi part);

   private:
    vector<int> pos;
    vector<int> b_pos;
    vector<float> vel;
    double max_vel = 3.0;
    double b_value;
    double value;
    double var_value;
    double max_inercia;
    double min_inercia;
    int dimension;
    int k_valor;
    Lector *lector;
    int cont;

    double calcularValor();
    void setMejorPosicion();
    vector<int> generarAleatorio();
};

#endif