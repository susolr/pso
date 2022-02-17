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
#include "lector.h"
#include <string>

using namespace std;

Paramlist::Paramlist(){

}

Paramlist::Paramlist(int argc, char* argv[]){

    string a="";
    for (int i = 1; i < argc; i++){
        a = a + argv[i];
    }
}