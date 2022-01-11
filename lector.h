/**
 * @file lector.h
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief Clase encargada del manejo de ficheros
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

protected:
    Lector();
    static Lector* lector;
public:
    Lector(Lector & otro) = delete;
    void operator =(const Lector & otro) = delete;
    void LeerTxt();
    vector<vector<double>> getDatos();
    void mostrarDatos();
    double getValor(int i, int j);
    static Lector *getInstance();
    
};



#endif