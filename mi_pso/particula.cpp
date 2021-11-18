/**
 * @file particula.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "particula.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

Particula::Particula(int n){
    dimension = n;
}

void Particula::actualizarPosicion(){

    double sig;
    for (int i = 0; i < vel.size(); i++){
        sig = 1.0/(1.0+exp(-vel.at(i)));
        if ((rand()%100) < sig){
            pos.at(i) = 1;
        }
        else {
            pos.at(i) = 0;
        }
    }

}

void Particula::actualizarVelocidad(vector<int> &g){

    vector<int> rd1;
    vector<int> rd2;

    for (int i = 0; i < dimension; i++){
        vel.at(i) = inercia*vel.at(i) + c_cog*rd1.at(i)*(b_pos.at(i) - pos.at(i)) + c_social*rd2.at(i)*(g.at(i) - pos.at(i));
    }

}

void Particula::valorar(){
    double aux_value = value;
    value = calcularValor();
    var_value = abs(value-aux_value);
    if (value > b_value){
        b_value = value;
        setMejorPosicion();
    }
}
double Particula::calcularValor(){
    return rand()%100;
}

void Particula::setMejorPosicion(){
    b_pos.clear();
    b_pos = pos;
}

double Particula::getValue(){
    return value;
}

double Particula::getBValue(){
    return b_value;
}

vector<int> Particula::getPos(){
    return pos;
}

vector<int> Particula::getBPos(){
    return b_pos;
}
