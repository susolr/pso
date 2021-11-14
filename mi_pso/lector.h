/**
 * @file lector.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef LECTOR_H
#define LECTOR_H

#include <vector>
using namespace std;

class Lector{
private:
    vector<vector<double>> datos;
    
public:
    Lector();
    void LeerTxt();
    vector<vector<double>> getDatos();
    void mostrarDatos();
    
};



#endif