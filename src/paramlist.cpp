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
 * @file paramlist.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Implementation of the ParamList object that manage the command line arguments
 * @copyright Atom (c) 2024 University of Granada
 */

#include "paramlist.h"

#include <mpi.h>
#include <omp.h>

#include <iostream>
#include <string>

#include "lector.h"
#include "parametro.h"
#include <unordered_map>

using namespace std;

Paramlist *Paramlist::paramlist = nullptr;

Paramlist::Paramlist() {
    init();
    setParametros();
}

Paramlist::Paramlist(int argc, char **argv) {
    init();
    for (int i = 1; i < argc; i += 2) {
        auto it = lista.find(argv[i]);
        if (it != lista.end()) {
            it->second.setValor(argv[i + 1]);
        }
    }
    int n_arg = (argc - 1) / 2;
    if (n_arg != lista.size()) {
        setParametros();
    }
}

void Paramlist::init() {
    lista["-nP"] = Parametro("-nP", "Parámetro para definir el número de partículas que conforman el cúmulo", "NParticulas");
    lista["-nH"] = Parametro("-nH", "Parámetro para definir el número de hebras en paralelo", to_string(omp_get_max_threads()), "NHebras");
    lista["-nI"] = Parametro("-nI", "Parámetro para definir el número máximo de iteraciones del bucle principal", "NMaxIteraciones");
    lista["-cS"] = Parametro("-cS", "Parámetro para definir el valor de la componente social del algoritmo", "CSocial");
    lista["-cC"] = Parametro("-cC", "Parámetro para definir el valor de la componente cognitiva del algoritmo", "CCognitiva");
    lista["-cI"] = Parametro("-cI", "Parámetro para definir el valor de la componente de inercia del algoritmo", "CInercia");
    lista["-k"] = Parametro("-k", "Parámetro para definir el valor K del algoritmo de clasificación", "KValor");
    lista["-sI"] = Parametro("-sI", "Parámetro para definir el uso de la orden simd", "SIMD");
    lista["-pC"] = Parametro("-pC", "Parámetro para definir la probabilidad de que la componente del vector de posición tome valor 1", "PCambio");
    lista["-bK"] = Parametro("-bK", "Parámetro para definir el uso del barrido de K", "BK");
    lista["-vI"] = Parametro("-vI", "Parámetro para definir el uso de la velocidad inicial", "VInicial");
    lista["-vC"] = Parametro("-vC", "Parámetro para definir el uso del control de velocidad", "VControl");
    lista["-iD"] = Parametro("-iD", "Parámetro para definir el uso de la inercia dinámica", "IDinamica");
    lista["-WSC"] = Parametro("-WSC", "Parámetro para definir el uso de websockets para la comunicacion", "WSComunicacion");
    lista["-WSR"] = Parametro("-WSR", "Parámetro para definir el uso de websockets para la recepcion de datos", "WSRealtime");
    lista["-cA"] = Parametro("-cA", "Parámetro para definir el uso de la componente aleatoria", "CAleatoria");
    lista["-dB"] = Parametro("-dB", "Parámetro para definir el uso de la base de datos", "DATABASE");
    lista["MPIrank"] = Parametro("MPIrank", "Parametro para guardar el rank de MPI del proceso", to_string(MPI::COMM_WORLD.Get_rank()), "MPIRank");
    lista["MPIsize"] = Parametro("MPIsize", "Parametro para guardar el tamaño de MPI COMM_WORLD", to_string(MPI::COMM_WORLD.Get_size()), "MPISize");
}

void Paramlist::setParametros() {
    for (auto& par : lista) {
        if (!par.second.isSet()) {
            Lector *lector = Lector::getInstance();
            par.second.setValor(lector->leerConfig(par.second.getTag()));
        }
    }
}

string Paramlist::getValor(string arg) {
    auto it = lista.find(arg);
    if (it == lista.end()) {
        return "";
    }
    return it->second.getValor();
}

void Paramlist::setValor(string tag, string valor) {
    auto it = lista.find(tag);
    if (it != lista.end()) {
        it->second.setValor(valor);
    }
}

unordered_map<string, string> Paramlist::getAllByTag() {
    unordered_map<string, string> out;
    for (const auto &kv : lista) {
        out[kv.first] = kv.second.getValor();
    }
    return out;
}

Paramlist *Paramlist::getInstance() {
    if (paramlist == nullptr) {
        paramlist = new Paramlist();
    }
    return paramlist;
}

Paramlist *Paramlist::getInstance(int argc, char **argv) {
    if (paramlist == nullptr) {
        paramlist = new Paramlist(argc, argv);
    }
    return paramlist;
}