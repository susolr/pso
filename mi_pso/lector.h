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