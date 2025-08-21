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
 * @file paramlist.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the ParamList object that manage the command line arguments
 * @copyright Atom (c) 2024 University of Granada
 */

#ifndef PARAMLIST_H
#define PARAMLIST_H

#include <string>
#include <vector>
#include <unordered_map>

#include "lector.h"
#include "parametro.h"
using namespace std;

class Paramlist {
   public:
    string getValor(string arg);
    void setValor(string tag, string valor);
    unordered_map<string, string> getAllByTag();
    Paramlist(Paramlist& otro) = delete;
    void operator=(const Paramlist& otro) = delete;
    static Paramlist* getInstance();
    static Paramlist* getInstance(int argc, char** argv);

   protected:
    Paramlist();
    Paramlist(int argc, char** argv);
    static Paramlist* paramlist;

   private:
    unordered_map<string, Parametro> lista;

    void init();
    // El método buscarParametro ya no es necesario con unordered_map
    void setParametros();
};
#endif