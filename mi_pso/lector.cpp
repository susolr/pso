/**
 * @file lector.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief Clase encargada del manejo de ficheros
 * @version 0.1
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "lector.h"
#include <iostream>
#include <fstream>

using namespace std;


Lector* Lector::lector= nullptr;

Lector::Lector(){

}

Lector* Lector::getInstance(){
    if(lector==nullptr){
        lector = new Lector();
    }
    return lector;
}

void Lector::LeerTxt(){

    ifstream f ("./datos.txt");
    vector<double> v;
    double dat;
    while (!f.eof()){
        for (int i = 0; i < 5; i++){
            f >> dat;
            v.push_back(dat);
        }
        datos.push_back(v);
        v.clear(); 
    }
    mostrarDatos();
}

vector<vector<double>> Lector::getDatos(){
    return datos;
}

double Lector::getValor(int i, int j){
    return (datos[i][j]);
}

void Lector::mostrarDatos(){
    for (int i = 0; i < datos.size(); ++i){
        for (int j = 0; j < datos.at(i).size(); ++j){
            cout << datos[i][j] << "\t";
        }
        cout << endl;
    }
}