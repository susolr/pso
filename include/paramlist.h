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
    string getValor(string arg);
    Paramlist (Paramlist & otro) = delete;
    void operator =(const Paramlist & otro) = delete;
    static Paramlist *getInstance();
    static Paramlist *getInstance(int argc, char** argv);

protected:
    Paramlist();
    Paramlist(int argc, char** argv);
    static Paramlist* paramlist;

private:
    vector<Parametro> lista;

    
    void init();
    int buscarParametro(string arg);
    void setParametros();

};
#endif