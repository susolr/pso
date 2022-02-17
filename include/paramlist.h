/**
 * @file paramlist.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef PARAMLIST_H
#define PARAMLIST_H

#include "lector.h"
#include "parametro.h"
#include <string>
#include <vector>
using namespace std;

class Paramlist{
public:
    Paramlist();
    Paramlist(int argc, char* argv[]);

private:
    vector<Parametro> lista;

    
    void init();
    int buscarParametro(string arg);
    void setParametros();

};
#endif