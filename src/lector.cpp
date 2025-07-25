/**
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of Atom repository.
 *
 * This work has been funded by:
 *
 * University of Granada under grant number PPJIA-2023-25.\n
 * Spanish 'Ministerio de Ciencia, Innovación y Universidades' under grants number
 * PID2022-137461NB-C32 and PID2020-119478GB-I00.\n Spanish 'Ministerio de Universidades' as part of
 * the program of mobility stays for professors and researchers in foreign higher education and
 * research centers under grant number CAS22/00332.\n European Regional Development Fund (ERDF).
 *
 * @file lector.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Implementation of the CSV reader
 * @copyright Atom (c) 2024 University of Granada
 */
#include "lector.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

Lector* Lector::lector = nullptr;

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

Lector::Lector() {
    // cout << "Creando Lector" << endl;
}

Lector* Lector::getInstance() {
    if (lector == nullptr) {
        lector = new Lector();
    }
    return lector;
}

void Lector::leerDatos() {
    leerDataTraining();
    leerDataTest();
    leerLabelsTraining();
    leerLabelsTest();
}

void Lector::leerDataTraining() {
    data_training.clear();
    string file = "./data/data_training_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    vector<double> v;
    double d;
    string line;
    int cont_col = 0;
    while (getline(f, line)) {
        vector<string> ss = split(line, ",");
        while ((cont_col < ss.size())) {
            d = stod(ss[cont_col]);
            v.push_back(d);
            cont_col++;
        }
        data_training.push_back(v);
        v.clear();
        cont_col = 0;
    }

    f.close();
}

void Lector::leerDataTest() {
    data_test.clear();
    string file = "./data/data_training_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    vector<double> v;
    double d;
    string line;
    int cont_col = 0;
    while (getline(f, line)) {
        vector<string> ss = split(line, ",");
        while ((cont_col < ss.size())) {
            d = stod(ss[cont_col]);
            v.push_back(d);
            cont_col++;
        }
        data_training.push_back(v);
        v.clear();
        cont_col = 0;
    }
    f.close();
}

void Lector::leerLabelsTraining() {
    labels_training.clear();
    string file = "./data/labels_training_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    int n;
    while (!f.eof()) {
        f >> n;
        labels_training.push_back(n);
    }
    f.close();
}

void Lector::leerLabelsTest() {
    labels_test.clear();
    string file = "./data/labels_test_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    int n;
    while (!f.eof()) {
        f >> n;
        labels_test.push_back(n);
    }
    f.close();
}

void Lector::leerDatos(int fil, int col) {
    leerDataTraining(fil, col);
    leerDataTest(fil, col);
    leerLabelsTraining(fil);
    leerLabelsTest(fil);
}

void Lector::leerDataTraining(int fil, int col) {
    data_training.clear();
    string file = "./data/data_training_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    vector<double> v;
    double d;
    string line;

    int cont_fil = 0;
    int cont_col = 0;
    while (getline(f, line) && (cont_fil < fil)) {
        // stringstream ss(line);
        // while ((ss >> d) && (cont_col < col)){
        vector<string> ss = split(line, ",");
        while ((cont_col < ss.size()) && (cont_col < col)) {
            d = stod(ss[cont_col]);
            v.push_back(d);
            cont_col++;
        }
        data_training.push_back(v);
        v.clear();
        cont_col = 0;
        cont_fil++;
    }
    f.close();
}

void Lector::leerDataTest(int fil, int col) {
    data_test.clear();
    string file = "./data/data_test_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    vector<double> v;
    double d;
    string line;
    int cont_fil = 0;
    int cont_col = 0;
    while (getline(f, line) && (cont_fil < fil)) {
        // stringstream ss(line);
        // while ((ss >> d) && (cont_col < col)){
        vector<string> ss = split(line, ",");
        while ((cont_col < ss.size()) && (cont_col < col)) {
            d = stod(ss[cont_col]);
            v.push_back(d);
            cont_col++;
        }
        data_test.push_back(v);
        v.clear();
        cont_col = 0;
        cont_fil++;
    }
    f.close();
}

void Lector::leerLabelsTraining(int fil) {
    labels_training.clear();
    string file = "./data/labels_training_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    int n;
    int cont = 0;
    while (!f.eof() && (cont < fil)) {
        f >> n;
        labels_training.push_back(n);
        cont++;
    }
    f.close();
}

void Lector::leerLabelsTest(int fil) {
    labels_test.clear();
    string file = "./data/labels_test_" + data_base + ".csv";
    // cout << "Leyendo fichero: " << file << endl;
    ifstream f(file);
    int n;
    int cont = 0;
    while (!f.eof() && (cont < fil)) {
        f >> n;
        labels_test.push_back(n);
        cont++;
    }
    f.close();
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

const vector<vector<double>>& Lector::getDataTest() const { return data_test; }

const vector<vector<double>>& Lector::getDataTraining() const { return data_training; }

const vector<int>& Lector::getLabelsTest() const { return labels_test; }

const vector<int>& Lector::getLabelsTraining() const { return labels_training; }

string Lector::getDataBase() { return data_base; }

/*double Lector::getValor(int i, int j){
    return (datos[i][j]);
}*/

void Lector::mostrarDatos() {
    mostrarDataTest();
    mostrarDataTraining();
    mostrarLabelsTest();
    mostrarLabelsTraining();
}

void Lector::mostrarDataTest() {
    for (int i = 0; i < data_test.size(); ++i) {
        for (int j = 0; j < data_test.at(i).size(); ++j) {
            cout << data_test[i][j] << "\t";
        }
        cout << endl;
    }
}

void Lector::mostrarDataTraining() {
    for (int i = 0; i < data_training.size(); ++i) {
        for (int j = 0; j < data_training.at(i).size(); ++j) {
            cout << data_training[i][j] << "\t";
        }
        cout << endl;
    }
}

void Lector::mostrarLabelsTest() {
    for (int i = 0; i < labels_test.size(); ++i) {
        cout << labels_test[i] << "\t";
    }
    cout << endl;
}

void Lector::mostrarLabelsTraining() {
    for (int i = 0; i < labels_training.size(); ++i) {
        cout << labels_training[i] << "\t";
    }
    cout << endl;
}

string Lector::leerConfig(string tag) {
    XMLDocument configDoc;
    configDoc.LoadFile("config.xml");
    XMLElement* root = configDoc.FirstChildElement();

    string param;
    param = root->FirstChildElement(tag.data())->GetText();

    return param;
}

void Lector::setDataBase(string db) { data_base = db; }
