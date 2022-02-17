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
    vector<int> getBPos();
    vector<int> getPos();


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
    int dimension;
    int k_valor;
    Lector * lector;
    vector<vector<double>> data_test;
    vector<vector<double>> data_training;
    vector<int> labels_test;
    vector<int> labels_training;

    double calcularValor();
    void setMejorPosicion();
    vector<int> generarAleatorio();

};


#endif