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
 * @file parametro.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the Parametro object
 * @copyright Atom (c) 2024 University of Granada
 */

#ifndef PARAMETRO_H
#define PARAMETRO_H

#include <string>

#include "lector.h"
using namespace std;

class Parametro {
   public:
    Parametro();
    Parametro(string arg, string descripcion, string tag);
    Parametro(string arg, string descripcion, string valor, string tag);

    // funciones de acceso y modificacion
    void setValor(string valor);
    void setTag(string tag);
    void setArg(string arg);
    void setDescripcion(string descripcion);
    string getValor() const;
    string getArg() const;
    string getDescipcion() const;
    string getTag() const;
    bool isSet();

   private:
    string arg;
    string descrip;
    string valor;
    string tag;
    bool set;
};

#endif