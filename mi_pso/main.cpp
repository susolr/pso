/**
 * @file main.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <ctime>
#include <cmath>
#include <queue>
#include "lector.h"

int main (int argc, char* argv[]){

    Lector * l = Lector::getInstance();

    l->LeerTxt();

    return 0;
}