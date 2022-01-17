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
    pos = generarAleatorio();
    c_cog = 0.8;
    c_social = 0.6;
    lector = Lector::getInstance();
    inercia = 0.8;
    for (int i = 0; i < dimension; i++){
        vel.push_back(0.0);
    }
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
    //cout << "Actualizando velocidad";
    vector<int> rd1 = generarAleatorio();
    vector<int> rd2 = generarAleatorio();

    for (int i = 0; i < dimension; i++){
        vel.at(i) = inercia*vel.at(i) + c_cog*rd1.at(i)*(b_pos.at(i) - pos.at(i)) + c_social*rd2.at(i)*(g.at(i) - pos.at(i));
    }

}

void Particula::valorar(){
    //cout << "Valorando" << endl;
    double aux_value = value;
    value = calcularValor();
    var_value = abs(value-aux_value);
    if (value > b_value){
        b_value = value;
        setMejorPosicion();
    }
}

/*double Particula::calcularValor(){
    vector<vector<double>> data = lector->getDataTest();
    double suma = 0.0;
    double res = 0.0;
    //cout << "Data size: " << data.size() << endl;
    for (int i = 1; i < data.size(); i++){
        //cout << "Primer for" << endl;
        for (int j = 0; j< data.at(i).size(); j++){
            //cout << "Segundo for" << endl;
            //cout << "Pos size " << pos.size() << endl;
            if (pos[j] == 1){
               // cout << "Paso " << i << " " << j << endl;
                double dis = data[0][j] - data[i][j];
                suma += dis*dis;
                //cout << "Distancia: " << dis << "\tSuma: " << suma << endl;
            }
            
        }
        
        suma = sqrt(suma);
        res += suma;
        suma = 0.0;
        //cout << "Resultado: " << res << "\n" << endl;
    }

    return res;
}*/

double Particula::calcularValor(){

    vector<vector<double>> data_test = lector->getDataTest();
    vector<vector<double>> data_training = lector->getDataTraining();
    vector<int> labels_test = lector->getLabelsTest();
    vector<int> labels_training = lector->getLabelsTraining();
    vector<pair<double, int>> distancias;
    for(int i = 0; i < data_test.size(); i++){
        for(int j = 0; j < data_training.size(); j++){
            for (int k = 0; k < data_training[i].size(); k++){

            }
        }
    }
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
