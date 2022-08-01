/**
 * @file main_knn.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"
#include "knn.h"
#include "omp.h"
//#include "paramlist.h"
using namespace std;

int main (int argc, char* argv[]){

    /*Paramlist * lista;
    if (argc==1){
        lista = Paramlist::getInstance();
    }
    else{
        lista = Paramlist::getInstance(argc, argv);
    }*/
    int k = *argv[1];
    KNN mi_knn = KNN(k);
    double time_inicio = omp_get_wtime();
    mi_knn.ejecutar();
    double time = omp_get_wtime() - time_inicio;
    mi_knn.mostrarResultados();
    cout << time*1000 << endl;


    return 0;
}