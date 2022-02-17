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
}

Paramlist::Paramlist(int argc, char* argv[]){
    init();
    string a="";
    for (int i = 1; i < argc; i++){
        a = a + argv[i];
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