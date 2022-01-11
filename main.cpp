/**
 * @file main.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"
#include "pso.h"

int main (int argc, char* argv[]){

/*
    PSO mi_pso = PSO();
    mi_pso.crearCumulo(20, 5);
    mi_pso.ejecutar();
    mi_pso.mostrarResultados();
*/
    Lector * lector = Lector::getInstance();
    vector<int> pos;
    vector<vector<double>> data = lector->getDatos();
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
    }

    return 0;
}