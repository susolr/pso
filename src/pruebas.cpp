/**
 * @file pruebas.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"
#include "pso.h"
#include "omp.h"

using namespace std;

int main (int argc, char* argv[]){


    string a="";
    for (int i = 1; i < argc; i+=2){
        cout << argv[i] << "\t" << argv[i+1] << endl;
    }

    return 0;
}