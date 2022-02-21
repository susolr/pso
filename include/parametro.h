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
    Parametro(string arg, string descripcion, string tag);
    Parametro(string arg, string descripcion, string valor, string tag);

    //funciones de acceso y modificacion
    void setValor(string valor);
    void setTag(string tag);
    void setArg(string arg);
    void setDescripcion(string descripcion);
    string getValor();
    string getArg();
    string getDescipcion();
    string getTag();
    bool isSet();

private:
    string arg;
    string descrip;
    string valor; 
    string tag;
    bool set;
};

#endif