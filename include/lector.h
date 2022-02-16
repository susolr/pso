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
#include <string>
using namespace std;

class Lector{
private:
    vector<vector<double>> data_test;
    vector<vector<double>> data_training;
    vector<int> labels_test;
    vector<int> labels_training;
    string data_base;


protected:
    Lector();
    static Lector* lector;
public:
    Lector(Lector & otro) = delete;
    void operator =(const Lector & otro) = delete;
    //void LeerTxt();

    //Métodos para leer la base de datos completa del fichero
    void leerDatos();
    void leerDataTraining();
    void leerDataTest();
    void leerLabelsTraining();
    void leerLabelsTest();

    //Métodos para leer una parte de la base de datos para hacer pruebas
    void leerDatos(int fil, int col);
    void leerDataTraining(int fil, int col);
    void leerDataTest(int fil, int col);
    void leerLabelsTraining(int fil);
    void leerLabelsTest(int fil);

    //Métodos para acceder a los distintos datos
    vector<vector<double>> getDataTest();
    vector<vector<double>> getDataTraining();
    vector<int> getLabelsTest();
    vector<int> getLabelsTraining();
    string getDataBase();

    //Métodos para mostrar los datos por pantalla
    void mostrarDatos();
    void mostrarDataTest();
    void mostrarDataTraining();
    void mostrarLabelsTest();
    void mostrarLabelsTraining();

    //Métodos para leer fichero config.xml
    void leerConfig();

    //double getValor(int i, int j);
    void setDataBase(string db);

    static Lector *getInstance();
    
    
};



#endif