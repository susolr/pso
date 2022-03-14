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
#include "tinyxml2.h"
#include "paramlist.h"

using namespace std;
using namespace tinyxml2;

int main (int argc, char** argv){

    int b_pos = 1;
    int pos = 0;
    int v_cog = (b_pos==pos) ? 1 : -1;

    cout << v_cog << endl;

    return 0;
}