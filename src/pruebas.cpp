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


/*    XMLDocument configDoc;
    configDoc.LoadFile("config.xml");
    XMLElement *root = configDoc.FirstChildElement();
    int nP;
    int nH;
    int nI;
    int k;
    root -> FirstChildElement("NParticulas") -> QueryIntText(&(nP));
    root -> FirstChildElement("NHebras") -> QueryIntText(&(nH));
    root -> FirstChildElement("NMaxIteraciones") -> QueryIntText(&(nI));
    root -> FirstChildElement("KValor") -> QueryIntText(&(k));
    cout << "NParticulas:" << nP << endl;
    cout << "NHebras:" << nH << endl;
    cout << "NMaxIteraciones:" << nI << endl;
    cout << "KValor:" << k << endl;
*/

    Paramlist lista = Paramlist();

    cout << lista.getValor("-nP") << endl;

    return 0;
}