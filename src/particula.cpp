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
#include "paramlist.h"


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
    //cout << "N_0 " << res[0] << "\tN_1 " << res[1] << "\tN_2 " << res[2] << "\tElegido " << label << endl;
    
    return label;
}

vector<int> barridoK(vector<pair<double,int>> distancias){
    
    vector<int> labels;

    for (int i = 0; i < 178; i++){
        int k = i+1;
        int l = valorKNN(k, distancias);
        labels.push_back(l);
    }
    
    return labels;
}

Particula::Particula(int n){
    dimension = n;
    pos = generarAleatorio();
    c_cog = stod(Paramlist::getInstance()->getValor("-cC"));
    c_social = stod((Paramlist::getInstance()->getValor("-cS")));
    lector = Lector::getInstance();
    inercia = stod(Paramlist::getInstance()->getValor("-cI"));
    b_value = 0.0;
    b_pos = pos;
    k_valor = stoi(Paramlist::getInstance()->getValor("-k"));
    simd_var = stoi((Paramlist::getInstance()->getValor("-sI")));
    n_hebras = stoi((Paramlist::getInstance()->getValor("-nH")));
    for (int i = 0; i < dimension; i++){
        vel.push_back(0.0);
    }
    data_test = lector->getDataTest();
    data_training = lector->getDataTraining();
    labels_test = lector->getLabelsTest();
    labels_training = lector->getLabelsTraining();
    //cout << "Sizes: " << data_test.size() << " " << data_training.size() << " " << labels_test.size() << " " << labels_training.size() << endl;
}

void Particula::actualizarPosicion(){
    //cout << "Actualizando posicion" << endl << flush;
    double sig;
    for (int i = 0; i < vel.size(); i++){
        sig = 1.0/(1.0+exp(-vel.at(i))); //SoftMax
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
    int k;
    value = calcularValor(k);
    var_value = abs(value-aux_value);
    if (value > b_value){
        b_value = value;
        setMejorPosicion();
        mejor_k = k;
    }
}

double Particula::calcularValor(){
    //cout << "Calcular valor" << endl;
    
    vector<int> labels_knn; 
    vector<pair<double, int>> distancias;
    //cout << "Size de test " << data_test.size();
    for(int i = 0; i < data_test.size(); i++){
        //cout << "Iteracion: \nI:" << i << endl;
        double distancia = 0.0;
        for(int j = 0; j < data_training.size(); j++){
            //cout << "Iteracion: \nI:" << i << "\nJ: " << j << endl;
        #pragma omp simd if(simd_var==1 && n_hebras>0)
            for (int k = 0; k < data_training[j].size(); k++){
                //cout << "Iteracion: \nI:" << i << "\nJ: " << j << "\nK:" << k << endl;
                if(pos[k]==1){
                    double aux = data_test[i][k] - data_training[j][k];
                    aux = aux*aux;
                    distancia += aux;
                }
            }
            distancia = sqrt(distancia);
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
    //cout << "Valor: " << valor << "\t N_aciertos " << n_aciertos << "\tN_fallos " << n_fallos << endl;
    return valor;

}

double Particula::calcularValor(int & k){
    //cout << "Calcular valor" << endl;
    
    //vector<int> labels_knn; 
    vector<vector<int>> labels_completos;
    vector<pair<double, int>> distancias;
    //cout << "Size de test " << data_test.size();
    for(int i = 0; i < data_test.size(); i++){
        //cout << "Iteracion: \nI:" << i << endl;
        double distancia = 0.0;
        for(int j = 0; j < data_training.size(); j++){
            //cout << "Iteracion: \nI:" << i << "\nJ: " << j << endl;
            for (int k = 0; k < data_training[j].size(); k++){
                //cout << "Iteracion: \nI:" << i << "\nJ: " << j << "\nK:" << k << endl;
                if(pos[k]==1){
                    double aux = data_test[i][k] - data_training[j][k];
                    aux = aux*aux;
                    distancia += aux;
                }
            }
            distancia = sqrt(distancia);
            pair<double,int> d_v (distancia, labels_training[j]);
            //cout << "Distancia: " << distancia << "\tValor " << labels_training[j] << endl;
            distancias.push_back(d_v);
            distancia = 0.0;
        }
        vector<int> valores_label_knn = barridoK(distancias);
        distancias.clear();
        labels_completos.push_back(valores_label_knn);
    }
    double b_valor = 0.0;    

    for (int j = 0; j < 178; j++){
        int n_aciertos = 0, n_fallos = 0;
        for (int i = 0; i < labels_completos.size(); i++){
            //cout << "LabelKnn: " << labels_knn[i] << "\t LabelTest " << labels_test[i] << endl;
            if (labels_completos[i][j] == labels_test[i]){
                n_aciertos++;
            }
            else{
                n_fallos++;
            }
        }
        double valor = ((n_aciertos)*1.0)/(1.0*(n_aciertos+n_fallos))*100;

        if (valor > b_valor){
            b_valor = valor;
            k = j+1;
        }
    }

    
    //cout << "Valor: " << valor << "\t N_aciertos " << n_aciertos << "\tN_fallos " << n_fallos << endl;
    return b_valor;

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

int Particula::getBK(){
    return mejor_k;
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
