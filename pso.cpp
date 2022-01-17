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
#include "lector.h"

using namespace std;

PSO::PSO(){
    Lector * lector = Lector::getInstance();
    lector->leerDatos();
    b_value = 0.0;
}

void PSO::crearCumulo(int n_particulas, int dimension){
    cout << "Creando cumulo" << endl;
    cumulo.clear();
    for (int i = 0; i < n_particulas; i++){
        cumulo.push_back(Particula(dimension));
        cout << "Creada partícula " << i << endl;
    }
    cout << "Tamaño del cúmulo: " << cumulo.size() << endl;
}

void PSO::ejecutar(){
    cout << "Bucle principal" << endl;
    int n_iter = 0;
    int contador = 0;
    while (n_iter < 10 && contador < 20){
        cout << "Iter: " << contador << endl;
        double aux_value = b_value, var_value;
        int max = 0;
        //cout << "Particula 0: " << cumulo[0].getPos()[2] << endl;
        cout << "Valorando..." << endl;
        for (int i = 0; i < cumulo.size(); i++){
            cumulo[i].valorar();
            if (cumulo[i].getValue() > b_value){
                b_value = cumulo[i].getValue();
                b_pos = cumulo[i].getPos();
            }
        }
        cout << "Valoradas todas las partículas" << endl;
        cout << "Actualizando velocidad y posicion..." << endl;
        for (int i = 0; i < cumulo.size(); i++){
            cumulo[i].actualizarVelocidad(b_pos);
            cumulo[i].actualizarPosicion();
        }
        cout << "Velocidad y posicion actualizadas" << endl;
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
    cout << "Monstrando resultados "  << endl;
    cout << "RESULTADOS:" << endl;
    cout << "Mejor valor: " << b_value << endl;
    cout << "Mejor posicion:";
    for (int i = 0; i < b_pos.size(); i++){
        cout << "\t" << b_pos[i];
    }
    cout << endl;

}