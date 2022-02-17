/**
 * @file parametro.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PARAMETRO_H
#define PARAMETRO_H

#include "lector.h"
#include <string>
using namespace std;

class Parametro{
public:
    Parametro();
    Parametro(string arg, string descripcion);
    Parametro(string arg, string descripcion, string valor);

    //funciones de acceso y modificacion
    void setValor(string valor);
    string getValor();
    string getArg();
    string getDescipcion();
    bool isSet();

private:
    string arg;
    string descrip;
    string valor; 
    bool set;
};

#endif