/**
 * @file knn.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "knn.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "lector.h"
#include <omp.h>
#include "paramlist.h"
#include <algorithm>

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

KNN::KNN(){
    lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDatos(178,3600);
    data_test = lector->getDataTest();
    data_training = lector->getDataTraining();
    labels_test = lector->getLabelsTest();
    labels_training = lector->getLabelsTraining();
    k_valor = stoi(Paramlist::getInstance()->getValor("-k"));
    valor = 0.0;

}

void KNN::ejecutar(){

    vector<int> labels_knn; 
    vector<pair<double, int>> distancias;
    //cout << "Size de test " << data_test.size();
    for(int i = 0; i < data_test.size(); i++){
        //cout << "Iteracion: \nI:" << i << endl;
        double distancia = 0.0;
        for(int j = 0; j < data_training.size(); j++){
            //cout << "Iteracion: \nI:" << i << "\nJ: " << j << endl;
            for (int k = 0; k < data_training[j].size(); k++){
                //cout << "Iteracion: \nI:" << i << "\nJ: " << j << "\nK:" << k << endl;
                double aux = data_test[i][k] - data_training[j][k];
                aux = aux*aux;
                distancia += aux;
                
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

    valor = ((n_aciertos)*1.0)/(1.0*(n_aciertos+n_fallos))*100;
    //cout << "Valor: " << valor << "\t N_aciertos " << n_aciertos << "\tN_fallos " << n_fallos << endl;


}

void KNN::mostrarResultados(){
    cout << valor << "\t" << k_valor << endl;
}