/**
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of Atom repository.
 *
 * This work has been funded by:
 *
 * University of Granada under grant number PPJIA-2023-25.\n
 * Spanish 'Ministerio de Ciencia, Innovación y Universidades' under grants number
 * PID2022-137461NB-C32 and PID2020-119478GB-I00.\n Spanish 'Ministerio de Universidades' as part of
 * the program of mobility stays for professors and researchers in foreign higher education and
 * research centers under grant number CAS22/00332.\n European Regional Development Fund (ERDF).
 *
 * @file lector.h
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Function declarations of the CSV reader
 * @copyright Atom (c) 2024 University of Granada
 */

#ifndef LECTOR_H
#define LECTOR_H

#include <string>
#include <vector>
using namespace std;

class Lector {
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
    Lector(Lector& otro) = delete;
    void operator=(const Lector& otro) = delete;
    // void LeerTxt();

    // Métodos para leer la base de datos completa del fichero
    void leerDatos();
    void leerDataTraining();
    void leerDataTest();
    void leerLabelsTraining();
    void leerLabelsTest();

    // Métodos para leer una parte de la base de datos para hacer pruebas
    void leerDatos(int fil, int col);
    void leerDataTraining(int fil, int col);
    void leerDataTest(int fil, int col);
    void leerLabelsTraining(int fil);
    void leerLabelsTest(int fil);

    // Métodos para acceder a los distintos datos
    const vector<vector<double>>& getDataTest() const;
    const vector<vector<double>>& getDataTraining() const;
    const vector<int>& getLabelsTest() const;
    const vector<int>& getLabelsTraining() const;
    string getDataBase();

    // Métodos para mostrar los datos por pantalla
    void mostrarDatos();
    void mostrarDataTest();
    void mostrarDataTraining();
    void mostrarLabelsTest();
    void mostrarLabelsTraining();

    // Métodos para leer fichero config.xml
    string leerConfig(string tag);

    // double getValor(int i, int j);
    void setDataBase(string db);

    static Lector* getInstance();
};

#endif