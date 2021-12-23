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
#include "lector.h"

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

    vector<int> rd1 = generarAleatorio();
    vector<int> rd2 = generarAleatorio();

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

    Lector * lector = Lector::getInstance();
    vector<vector<double>> data= lector->getDatos();
    double suma = 0.0;
    double res = 0.0;
    for (int i = 1; i < data.size(); i++){
        for (int j = 0; j< data.at(i).size(); j++){
            if (pos[j] == 1){
                double dis = data[0][j] - data[i][j];
                suma += dis*dis;
            }
            
        }
        suma = sqrt(suma);
        res += suma;
        suma = 0.0;
    }
    
    return res;
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

vector<int> Particula::generarAleatorio(){
    vector<int> aux;

    for (int i = 0; i < dimension; ++i){
        
        if (rand()%100 >= 0.5){
            aux.push_back(1);
        }
        else {
            aux.push_back(0);
        }
    }

    return aux;
}
