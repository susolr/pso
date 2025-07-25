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
 * @file particula.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Implementation of the Particula object
 * @copyright Atom (c) 2024 University of Granada
 */

#include "particula.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "lector.h"
#include "paramlist.h"

using namespace std;

int valorKNN(int k_valor, vector<pair<double, int>> distancias) {
    vector<int> res = {0, 0, 0, 0};
    int label;

    sort(distancias.begin(), distancias.end());

    for (int i = 0; i < k_valor; i++)

    {
        int l = distancias[i].second;
        // cout << "Pos: " << i << " " << distancias[i].first << " " <<
        // distancias[i].second << endl;
        res[l]++;
    }

    // sort(res.begin(), res.end());
    int max = res[0];
    int pos_max = 0;

    for (int i = 1; i < res.size(); i++) {
        if (res[i] > max) {
            max = res[i];
            pos_max = i;
        }
    }

    label = pos_max;
    // cout << "N_0 " << res[0] << "\tN_1 " << res[1] << "\tN_2 " << res[2] <<
    // "\tElegido " << label << endl;

    return label;
}

Particula::Particula() {
    dimension = N_FEATURES;
    for (int i = 0; i < dimension; i++) {
        double var_rand = (rand() % 10000) * 1.0 / 10000.0;
        if (var_rand > stod(Paramlist::getInstance()->getValor("-pC"))) {
            pos.push_back(1);
        } else {
            pos.push_back(0);
        }
    }

    lector = Lector::getInstance();
    max_inercia = stod(Paramlist::getInstance()->getValor("-cI"));

    b_value = 0.0;
    b_pos = pos;

    for (int i = 0; i < dimension; i++) {
        if (stoi(Paramlist::getInstance()->getValor("-vI")) == 1) {
            double v = (rand() % 200 - 100) * 1.0 / 100.0;
            vel.push_back(v);
        } else {
            vel.push_back(0.0);
        }
    }
    cont = 0;
}

Particula::Particula(int n) {
    dimension = n;
    for (int i = 0; i < dimension; i++) {
        double var_rand = (rand() % 10000) * 1.0 / 10000.0;
        if (var_rand > stod(Paramlist::getInstance()->getValor("-pC"))) {
            pos.push_back(1);
        } else {
            pos.push_back(0);
        }
    }

    lector = Lector::getInstance();
    max_inercia = stod(Paramlist::getInstance()->getValor("-cI"));

    b_value = 0.0;
    b_pos = pos;
    int v_inicial = stoi(Paramlist::getInstance()->getValor("-vI"));
    for (int i = 0; i < dimension; i++) {
        if (v_inicial == 1) {
            double v = (rand() % 200 - 100) * 1.0 / 100.0;
            vel.push_back(v);
        } else {
            vel.push_back(0.0);
        }
    }
    // vel = generarAleatorio();
    cont = 0;
}

// Formula estándar para actualizar la posicion
void Particula::actualizarPosicion() {
    // cout << "Actualizando posicion" << endl << flush;
    double sig;
    for (int i = 0; i < vel.size(); i++) {
        sig = 1.0 / (1.0 + exp(-vel.at(i)));  // Sigmoidea
        // cout << "sig: " << sig << endl;
        // double var_rand = (rand() % 10000) * 1.0 / 10000.0;
        if (stod(Paramlist::getInstance()->getValor("-pC")) < sig) {
            pos.at(i) = 1;
        } else {
            pos.at(i) = 0;
        }
    }
}

void Particula::actualizarVelocidad(vector<int> &g) {
    // cout << "Actualizando velocidad" << endl << flush;
    vector<int> rd1 = generarAleatorio();
    vector<int> rd2 = generarAleatorio();

    for (int i = 0; i < dimension; i++) {
        int componente_aleatoria = stoi(Paramlist::getInstance()->getValor("-vC"));
        if (componente_aleatoria == 1) {
            vel.at(i) = stod(Paramlist::getInstance()->getValor("-cI")) * vel.at(i) +
                        stod(Paramlist::getInstance()->getValor("-cC")) * rd1.at(i) *
                            (b_pos.at(i) - pos.at(i)) +
                        stod((Paramlist::getInstance()->getValor("-cS"))) * rd2.at(i) *
                            (g.at(i) - pos.at(i));  // Con componente aleatoria
        } else {
            vel.at(i) =
                stod(Paramlist::getInstance()->getValor("-cI")) * vel.at(i) +
                stod(Paramlist::getInstance()->getValor("-cC")) * (b_pos.at(i) - pos.at(i)) +
                stod((Paramlist::getInstance()->getValor("-cS"))) *
                    (g.at(i) - pos.at(i));  // Sin componente aleatoria. No satisfactorio
        }

        int control_velocidad = stoi(Paramlist::getInstance()->getValor("-vC"));
        if (control_velocidad == 1) {
            if (vel.at(i) > 3.0) {
                vel.at(i) = 3.0;
            }
            if (vel.at(i) < -3.0) {
                vel.at(i) = -3.0;
            }
        }
    }
    int inercia_dinamica = stoi(Paramlist::getInstance()->getValor("-iD"));

    if (inercia_dinamica == 1) {
        cont++;
        Paramlist::getInstance()->getValor("-cI") =
            max_inercia -
            ((max_inercia - 0.1) / stoi((Paramlist::getInstance()->getValor("-nI")))) * cont;
    }
}

void Particula::valorar() {
    // cout << "Valorando" << endl;
    double aux_value = value;
    value = calcularValor();
    var_value = abs(value - aux_value);
    if (value > b_value) {
        b_value = value;
        setMejorPosicion();
    }
}

double Particula::calcularValor() {
    // cout << "Calcular valor" << endl;

    vector<int> labels_knn;
    vector<pair<double, int>> distancias;
    for (int i = 0; i < lector->getDataTest().size(); i++) {
        // cout << "Iteracion: \nI:" << i << endl;
        double distancia = 0.0;
        for (int j = 0; j < lector->getDataTraining().size(); j++) {
            // cout << "Iteracion: \nI:" << i << "\nJ: " << j << endl;
#pragma omp simd
            for (int k = 0; k < lector->getDataTraining()[j].size(); k++) {
                // cout << "Iteracion: \nI:" << i << "\nJ: " << j << "\nK:" << k <<
                // endl;
                if (pos[k] == 1) {
                    double aux = lector->getDataTest()[i][k] - lector->getDataTraining()[j][k];
                    aux = aux * aux;
                    distancia += aux;
                }
            }
            distancia = sqrt(distancia);
            pair<double, int> d_v(distancia, lector->getLabelsTraining()[j]);
            // cout << "Distancia: " << distancia << "\tValor " <<
            // lector->getLabelsTraining()[j] << endl;
            distancias.push_back(d_v);
            distancia = 0.0;
        }
        int valor_label_knn = valorKNN(stoi(Paramlist::getInstance()->getValor("-k")), distancias);
        distancias.clear();
        labels_knn.push_back(valor_label_knn);
    }

    int n_aciertos = 0, n_fallos = 0;

    for (int i = 0; i < labels_knn.size(); i++) {
        // cout << "LabelKnn: " << labels_knn[i] << "\t LabelTest " <<
        // lector->getLabelsTest()[i] << endl;
        if (labels_knn[i] == lector->getLabelsTest()[i]) {
            n_aciertos++;
        } else {
            n_fallos++;
        }
    }

    double valor = ((n_aciertos) * 1.0) / (1.0 * (n_aciertos + n_fallos)) * 100;
    // cout << "Valor: " << valor << "\t N_aciertos " << n_aciertos << "\tN_fallos
    // " << n_fallos << endl;
    return valor;
}

void Particula::setMejorPosicion() {
    b_pos.clear();
    b_pos = pos;
}

double Particula::getValue() { return value; }

double Particula::getBValue() { return b_value; }

void Particula::setValue(double val) {
    cout << "Llamando a setValue con " << value << "\t" << val << endl;
    value = val;
    if (value > b_value) {
        b_value = value;
        setMejorPosicion();
    }
}

vector<int> Particula::getPos() { return pos; }

void Particula::setPos(vector<int> npos) { pos = npos; }

vector<int> Particula::getBPos() { return b_pos; }

vector<int> Particula::generarAleatorio() {
    vector<int> aux;
    for (int i = 0; i < dimension; ++i) {
        /*
        if (rand()%100 >= 0.5){
            aux.push_back(1);
        }
        else {
            aux.push_back(0);
        }*/
        aux.push_back(rand() % 2);
    }

    return aux;
}

particula_mpi Particula::toStruct() {
    particula_mpi aux;
    for (int i = 0; i < dimension; i++) {
        aux.pos[i] = pos.at(i);
    }

    return aux;
}

void Particula::fromStruct(particula_mpi part) {
    for (int i = 0; i < dimension; i++) {
        pos[i] = part.pos[i];
    }
}
