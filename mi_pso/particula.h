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
using namespace std;

class Particula {
public:
    Particula(int n);
    void actualizarPosicion();
    void valorar();
    void actualizarVelocidad(vector<int>& g);


private: 
    vector<int> pos;
    vector<int> b_pos;
    vector<double> vel;
    double max_vel = 3.0;
    double b_value;
    double value;
    double c_cog;
    double c_social;
    double inercia;
    int dimension;

    double calcularValor();
    void setMejorPosicion();


};


#endif