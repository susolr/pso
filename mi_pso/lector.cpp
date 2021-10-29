#include "lector.h"
#include <iostream>
#include <fstream>

using namespace std;

Lector::Lector(){

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

void Lector::mostrarDatos(){
    for (int i = 0; i < datos.size(); ++i){
        for (int j = 0; j < datos.at(i).size(); ++j){
            cout << datos[i][j] << "\t";
        }
        cout << endl;
    }
}