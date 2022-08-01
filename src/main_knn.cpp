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
    double time_inicio = omp_get_wtime();
    for (int i = 1; i <= 178; i++){
        KNN mi_knn = KNN(i);        
        mi_knn.ejecutar();       
        mi_knn.mostrarResultados();       
    }
    double time = omp_get_wtime() - time_inicio;
    cout << time*1000 << endl;

    return 0;
}