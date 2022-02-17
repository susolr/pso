/**
 * @file pruebas.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"
#include "pso.h"
#include "omp.h"

using namespace std;

int main (int argc, char* argv[]){

    Lector * lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDatos(178,3600);

    vector<int> a = lector->getLabelsTest();

    cout << "Labels size: " << a.size() << endl;
    for (int i = 0; i < a.size(); i++){
        cout << a.at(i) << "\t" << endl;
    }
    cout << "fin" << endl;
    return 0;
}