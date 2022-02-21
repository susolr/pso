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
    tag = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion, string tag){
    this->arg = arg;
    this->descrip = descripcion;
    this->tag = tag;
    valor = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion, string valor, string tag){
    this->arg = arg;
    this->descrip = descripcion;
    this->valor = valor;
    this->tag = tag;
    set = true;
}

bool Parametro::isSet(){
    return set;
}

void Parametro::setValor(string valor){
    this->valor = valor;
    set = true;
}

void Parametro::setTag(string tag){
    this->tag = tag;
}

void Parametro::setArg(string arg){
    this->arg = arg;
}

void Parametro::setDescripcion(string descripcion){
    this->descrip = descripcion;
}

string Parametro::getValor(){
    return valor;
}

string Parametro::getArg(){
    return arg;
}

string Parametro::getDescipcion(){
    return descrip;
}

string Parametro::getTag(){
    return tag;
}