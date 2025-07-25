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
 * @file parametro.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Implementation of the Param object
 * @copyright Atom (c) 2024 University of Granada
 */

#include "parametro.h"

using namespace std;

Parametro::Parametro() {
    arg = "";
    descrip = "";
    valor = "";
    tag = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion, string tag) {
    this->arg = arg;
    this->descrip = descripcion;
    this->tag = tag;
    valor = "";
    set = false;
}

Parametro::Parametro(string arg, string descripcion, string valor, string tag) {
    this->arg = arg;
    this->descrip = descripcion;
    this->valor = valor;
    this->tag = tag;
    set = true;
}

bool Parametro::isSet() { return set; }

void Parametro::setValor(string valor) {
    this->valor = valor;
    set = true;
}

void Parametro::setTag(string tag) { this->tag = tag; }

void Parametro::setArg(string arg) { this->arg = arg; }

void Parametro::setDescripcion(string descripcion) { this->descrip = descripcion; }

string Parametro::getValor() { return valor; }

string Parametro::getArg() { return arg; }

string Parametro::getDescipcion() { return descrip; }

string Parametro::getTag() { return tag; }