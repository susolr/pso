#include "particula.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

Particula::Particula(){

}

void Particula::actualizarPosicion(){

    double sig;
    for (int i = 0; i < vel.size(); i++){
        sig = 1.0/(1.0+exp(-vel.at(i)));
        if (rand() < sig){
            pos.at(i) = 1;
        }
        else {
            pos.at(i) = 0;
        }
    }

}

void Particula::actualizarVelocidad(){

}

void Particula::valorar(){

}
