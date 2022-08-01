/**
 * @file knn.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef KNN_H
#define KNN_H

#include <vector>
#include "lector.h"
using namespace std;


class KNN {
public:
    KNN(int k);
    void ejecutar();
    void mostrarResultados();

private:
    Lector * lector;
    vector<vector<double>> data_test;
    vector<vector<double>> data_training;
    vector<int> labels_test;
    vector<int> labels_training;
    double valor;
    int k_valor;

};


#endif