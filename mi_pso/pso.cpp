/**
 * @file pso.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "pso.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

PSO::PSO(){
}

void PSO::crearCumulo(int n_particulas, int dimension){
    cumulo.clear();
    for (int i = 0; i < n_particulas; i++){
        cumulo.push_back(Particula(dimension));
    }
}

void PSO::ejecutar(){
    int n_iter = 0;
    int contador = 0;
    while (n_iter < 10 && contador <=20){
        double aux_value = b_value, var_value;
        int max = 0;
        for (int i = 0; i < cumulo.size(); i++){
            cumulo.at(i).valorar();
            if (cumulo.at(i).getValue() > b_value){
                b_value = cumulo.at(i).getValue();
                b_pos = cumulo.at(i).getPos();
            }
        }

        for (int i = 0; i < cumulo.size(); i++){
            cumulo.at(i).actualizarVelocidad(b_pos);
            cumulo.at(i).actualizarPosicion();
        }
        var_value = abs(b_value-aux_value);

        if (var_value < pow(10,-3)){
            ++n_iter;
        }
        else {
            n_iter = 0;
        }
        contador++;
    }
}

void PSO::mostrarResultados(){

    cout << "RESULTADOS:" << endl;
    cout << "Mejor valor: " << b_value << endl;
    cout << "Mejor posicion:";
    for (int i = 0; i < b_pos.size(); i++){
        cout << "\t" << b_pos[i];
    }
    cout << endl;

}