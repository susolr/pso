/**
 * @file paramlist.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "paramlist.h"
#include "parametro.h"
#include "lector.h"
#include <string>

using namespace std;

Paramlist::Paramlist(){
    init();
    //Load and set args
}

Paramlist::Paramlist(int argc, char* argv[]){
    init();
    for (int i = 1; i < argc; i++){
        int pos = buscarParametro(argv[i]);
        //Comprobar valor adecuado??
        lista.at(pos).setValor(argv[i+1]);
    }
    int n_arg = (argc-1)/2;
    if ( n_arg != lista.size()){
        //Load and set args
    }
}

void Paramlist::init(){
    lista.push_back(Parametro("-nP", "Parámetro para definir el número de párticulas que conforman el cúmulo"));
    lista.push_back(Parametro("-nH", "Parámetro para definir el número de hebras en paralelo"));
    lista.push_back(Parametro("-nI", "Parámetro para definir el número máximo de iteraciones del bucle principal"));
    lista.push_back(Parametro("-cS", "Parámetro para definir el valor de la componente social del algoritmo"));
    lista.push_back(Parametro("-cC", "Parámetro para definir el valor de la componente cognitiva del algoritmo"));
    lista.push_back(Parametro("-cI", "Parámetro para definir el valor de la componente de inercia del algoritmo"));
    lista.push_back(Parametro("-k" , "Parámetro para definir el valor K del algoritmo de clasificación"));
}

int Paramlist::buscarParametro(string arg){
    int pos = -1;
    for (int i = 0; i < lista.size(); i++){
        if (arg.compare(lista.at(i).getArg()) == 0){
            pos = i;
        }
    }

    return pos;
}