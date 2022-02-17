/**
 * @file parametro.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "parametro.h"

using namespace std;

Parametro::Parametro(){
    arg = "";
    descrip = "";
    valor = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion){
    this->arg = arg;
    this->descrip = descripcion;
    valor = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion, string valor){
    this->arg = arg;
    this->descrip = descripcion;
    this->valor = valor;
    set = true;
}

bool Parametro::isSet(){
    return set;
}

void Parametro::setValor(string valor){
    this->valor = valor;
    set = true;
}

string Parametro::getValor(){
    return valor;
}
