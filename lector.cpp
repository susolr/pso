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
#include <string>

using namespace std;


Lector* Lector::lector= nullptr;

Lector::Lector(){
    cout << "Creando Lector" << endl;
}

Lector* Lector::getInstance(){
    if(lector==nullptr){
        lector = new Lector();
    }
    return lector;
}

void Lector::leerDatos(){
    leerDataTraining();
    leerDataTest();
    leerLabelsTraining();
    leerLabelsTest();
}

void Lector::leerDataTraining(){

}

void Lector::leerDataTest(){

}

void Lector::leerLabelsTraining(){
    labels_training.clear();
    string file = "./labels_training_" + data_base + ".csv";
    cout << "Leyendo fichero: " << file << endl;
    ifstream f (file);
    int n;
    while (!f.eof()){
        f >> n;
        labels_training.push_back(n);
    }
}

void Lector::leerLabelsTest(){

}

void Lector::leerDatos(int fil, int col){
    leerDataTraining(fil, col);
    leerDataTest(fil, col);
    leerLabelsTraining(fil);
    leerLabelsTest(fil);
}

void Lector::leerDataTraining(int fil, int col){

}

void Lector::leerDataTest(int fil, int col){

}

void Lector::leerLabelsTraining(int fil){
    string file = "./labels_training" + data_base + ".csv";
    ifstream f (file);
    int n;
    int cont = 0;
    while (!f.eof() && (cont<fil)){
        f >> n;
        labels_training.push_back(n);
    }
}

void Lector::leerLabelsTest(int fil){
    
}

/*void Lector::LeerTxt(){

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
    //mostrarDatos();
}
*/

vector<vector<double>> Lector::getDataTest(){
    return data_test;
}

vector<vector<double>> Lector::getDataTraining(){
    return data_training;
}

vector<int> Lector::getLabelsTest(){
    return labels_test;
}

vector<int> Lector::getLabelsTraining(){
    return labels_training;
}

string Lector::getDataBase(){
    return data_base;
}

/*double Lector::getValor(int i, int j){
    return (datos[i][j]);
}*/

void Lector::mostrarDatos(){
    mostrarDataTest();
    mostrarDataTraining();
    mostrarLabelsTest();
    mostrarLabelsTraining();

}

void Lector::mostrarDataTest(){
    for (int i = 0; i < data_test.size(); ++i){
        for (int j = 0; j < data_test.at(i).size(); ++j){
            cout << data_test[i][j] << "\t";
        }
        cout << endl;
    }
}

void Lector::mostrarDataTraining(){
    for (int i = 0; i < data_training.size(); ++i){
        for (int j = 0; j < data_training.at(i).size(); ++j){
            cout << data_training[i][j] << "\t";
        }
        cout << endl;
    }
}

void Lector::mostrarLabelsTest(){
    for (int i = 0; i < labels_test.size(); ++i){
        cout << labels_test[i] << "\t";
    }
    cout << endl;
}

void Lector::mostrarLabelsTraining(){
    for (int i = 0; i < labels_training.size(); ++i){
        cout << labels_training[i] << "\t";
    }
    cout << endl;
}

void Lector::setDataBase(string db){
    data_base = db;
}