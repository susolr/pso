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
#include <omp.h>
#include "paramlist.h"

using namespace std;

PSO::PSO(){
    Lector * lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDatos(178,3600);
    dimension = 3600;
    //lector->mostrarDatos();
    b_value = 0.0;
}

void PSO::crearCumulo(){
    cout << "Creando cumulo" << endl;
    cumulo.clear();
    int n_particulas = stoi(Paramlist::getInstance()->getValor("-nP"));
    for (int i = 0; i < n_particulas; i++){
        cumulo.push_back(Particula(dimension));
        //cout << "Creada partícula " << i << endl;
    }
    cout << "Tamaño del cúmulo: " << cumulo.size() << endl;
    b_pos = cumulo[0].getPos();
}

void PSO::ejecutar(){
    //cout << "Bucle principal" << endl;
    int n_iter = 0;
    int contador = 0;
    int n_threads = stoi(Paramlist::getInstance()->getValor("-nH"));
    while (n_iter < 10 && contador < 20){
        //cout << "Iter: " << contador << endl;
        double aux_value = b_value, var_value;
        int max = 0;
        //cout << "Particula 0: " << cumulo[0].getPos()[2] << endl;
        //cout << "Valorando..." << endl;
        #pragma opm parallel for num_threads(n_threads)
        for (int i = 0; i < cumulo.size(); i++){
            cumulo[i].valorar();
        }
        bool cambia = false;
        int pos;
        //cout << "Antes de actualizacion b_pos" << endl;
        for (int i = 0; i < cumulo.size(); i++){
            if (cumulo[i].getBValue() > b_value){
                cambia = true;
                b_value = cumulo[i].getBValue();
                pos = i;
            }
        }
        if (cambia){
                b_pos.clear();
                b_pos = cumulo[pos].getBPos();
        }
        //cout << "Valoradas todas las partículas" << endl;
        //cout << "Actualizando velocidad y posicion..." << endl;
        #pragma omp parallel for num_threads(n_threads)
        for (int i = 0; i < cumulo.size(); i++){
            //cout << "Actualizando: " << i << endl;
            //cout << "Tamaño del cúmulo " << cumulo.size() << endl;
            //cout << "Tamaño de b_pos " << b_pos.size() << endl;
            cumulo[i].actualizarVelocidad(b_pos);
            cumulo[i].actualizarPosicion();
        }
        //cout << "Velocidad y posicion actualizadas" << endl;
        var_value = abs(b_value-aux_value);

        if (var_value < 0.001){
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
    cout << "Mejor valor: " << b_value << "%" << endl;
    //cout << "Mejor posicion:";
    /*for (int i = 0; i < b_pos.size(); i++){
        cout << "\t" << b_pos[i];
    }*/
    cout << endl;

}