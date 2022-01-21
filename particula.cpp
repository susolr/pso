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
#include <algorithm>
#include "lector.h"


using namespace std;

int valorKNN(int k_valor, vector<pair<double,int>> distancias){
    vector<int> res = {0,0,0};
    int label;

    sort(distancias.begin(), distancias.end());

    for (int i = 0; i < k_valor; i++){
        int l = distancias[i].second;
        //cout << "Pos: " << i << " " << distancias[i].first << " " << distancias[i].second << endl;
        res[l]++;
    }

    //sort(res.begin(), res.end());
    int max = res[0];
    int pos_max = 0;

    for (int i = 1; i < res.size(); i++){
        if (res[i] > max){
            max = res[i];
            pos_max = i;
        }
    }

    label = pos_max;
    cout << "N_0 " << res[0] << "\tN_1 " << res[1] << "\tN_2 " << res[2] << "\tElegido " << label << endl;
    
    return label;
}

Particula::Particula(int n){
    dimension = n;
    pos = generarAleatorio();
    c_cog = 0.8;
    c_social = 0.6;
    lector = Lector::getInstance();
    inercia = 0.8;
    b_value = 0.0;
    b_pos = pos;
    for (int i = 0; i < dimension; i++){
        vel.push_back(0.0);
    }
}

void Particula::actualizarPosicion(){
    //cout << "Actualizando posicion" << endl << flush;
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
    //cout << "Actualizando velocidad" << endl << flush;
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

double Particula::calcularValor(){
    //cout << "Calcular valor" << endl;
    vector<vector<double>> data_test = lector->getDataTest();
    vector<vector<double>> data_training = lector->getDataTraining();
    vector<int> labels_test = lector->getLabelsTest();
    vector<int> labels_training = lector->getLabelsTraining();
    vector<int> labels_knn; 
    vector<pair<double, int>> distancias;
    int k_valor = 7;
    //cout << "Size de test " << data_test.size();
    for(int i = 0; i < data_test.size(); i++){
        //cout << "Iteracion: \nI:" << i << endl;
        double distancia = 0.0;
        for(int j = 0; j < data_training.size(); j++){
            //cout << "Iteracion: \nI:" << i << "\nJ: " << j << endl;
            for (int k = 0; k < data_training[i].size(); k++){
                //cout << "Iteracion: \nI:" << i << "\nJ: " << j << "\nK:" << k << endl;
                if(pos[k]==1){
                    double aux = data_test[i][k] - data_training[j][k];
                    aux = aux*aux;
                    distancia += aux;
                }
            }
            pair<double,int> d_v (distancia, labels_training[j]);
            //cout << "Distancia: " << distancia << "\tValor " << labels_training[j] << endl;
            distancias.push_back(d_v);
            distancia = 0.0;
        }
        int valor_label_knn = valorKNN(k_valor, distancias);
        distancias.clear();
        labels_knn.push_back(valor_label_knn);
    }

    int n_aciertos = 0, n_fallos = 0;

    for (int i = 0; i < labels_knn.size(); i++){
        //cout << "LabelKnn: " << labels_knn[i] << "\t LabelTest " << labels_test[i] << endl;
        if (labels_knn[i] == labels_test[i]){
            n_aciertos++;
        }
        else{
            n_fallos++;
        }
    }

    double valor = ((n_aciertos)*1.0)/(1.0*(n_aciertos+n_fallos))*100;
    cout << "Valor: " << valor << "\t N_aciertos " << n_aciertos << "\tN_fallos " << n_fallos << endl;
    return valor;

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
        /*
        if (rand()%100 >= 0.5){
            aux.push_back(1);
        }
        else {
            aux.push_back(0);
        }*/
        aux.push_back(rand()%2);
    }

    return aux;
}
