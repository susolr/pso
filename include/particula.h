/**
 * @file particula.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef PARTICULA_H
#define PARTICULA_H

#include <vector>
#include "lector.h"
using namespace std;

class Particula {
public:
    Particula(int n);
    void actualizarPosicion();
    void valorar();
    void actualizarVelocidad(vector<int>& g);
    double getValue();
    double getBValue();
    void setPos (vector<int> npos);
    vector<int> getBPos();
    vector<int> getPos();
    int getBK();


private: 
    vector<int> pos;
    vector<int> b_pos;
    vector<double> vel;
    double max_vel = 3.0;
    double b_value;
    double value;
    double var_value;
    double c_cog;
    double c_social;
    double inercia;
    double max_inercia;
    double min_inercia;
    int dimension;
    int k_valor;
    int simd_var;
    int n_hebras;
    int max_iter;
    Lector * lector;
    vector<vector<double>> data_test;
    vector<vector<double>> data_training;
    vector<int> labels_test;
    vector<int> labels_training;
    int mejor_k;
    int cont;

    double calcularValor();
    double calcularValor(int & k);
    void setMejorPosicion();
    vector<int> generarAleatorio();

};


#endif