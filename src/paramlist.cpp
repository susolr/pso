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
#include "parametro.h"
#include <iostream>
#include "lector.h"
#include <string>
#include <mpi.h>
#include <omp.h>

using namespace std;

Paramlist *Paramlist::paramlist = nullptr;

Paramlist::Paramlist()
{
    init();
    // Load and set args
    setParametros();
}

Paramlist::Paramlist(int argc, char **argv)
{
    init();
    for (int i = 1; i < argc; i += 2)
    {
        int pos = buscarParametro(argv[i]);
        // Comprobar valor adecuado??
        lista[pos].setValor(argv[i + 1]);
    }
    int n_arg = (argc - 1) / 2;
    if (n_arg != lista.size())
    {
        // Load and set args
        setParametros();
    }
}

void Paramlist::init()
{
    lista.push_back(Parametro("-nP", "Parámetro para definir el número de párticulas que conforman el cúmulo", "NParticulas"));
    lista.push_back(Parametro("-nH", "Parámetro para definir el número de hebras en paralelo", to_string(omp_get_max_threads()), "NHebras"));
    lista.push_back(Parametro("-nI", "Parámetro para definir el número máximo de iteraciones del bucle principal", "NMaxIteraciones"));
    lista.push_back(Parametro("-cS", "Parámetro para definir el valor de la componente social del algoritmo", "CSocial"));
    lista.push_back(Parametro("-cC", "Parámetro para definir el valor de la componente cognitiva del algoritmo", "CCognitiva"));
    lista.push_back(Parametro("-cI", "Parámetro para definir el valor de la componente de inercia del algoritmo", "CInercia"));
    lista.push_back(Parametro("-k", "Parámetro para definir el valor K del algoritmo de clasificación", "KValor"));
    lista.push_back(Parametro("-sI", "Parámetro para definir el uso de la orden simd", "SIMD"));
    lista.push_back(Parametro("-pC", "Parámetro para definir la probabilidad de que la componente del vector de posición tome valor 1", "PCambio"));
    lista.push_back(Parametro("MPIrank", "Parametro para guardar el rank de MPI del proceso", to_string(MPI::COMM_WORLD.Get_rank()), "MPIRank"));
    lista.push_back(Parametro("MPIsize", "Parametro para guardar el tamaño de MPI COMM_WORLD", to_string(MPI::COMM_WORLD.Get_size()), "MPISize"));
}

int Paramlist::buscarParametro(string arg)
{
    int pos = -1;
    for (int i = 0; i < lista.size(); i++)
    {
        if (arg.compare(lista[i].getArg()) == 0)
        {
            pos = i;
        }
    }
    return pos;
}

void Paramlist::setParametros()
{
    for (int i = 0; i < lista.size(); i++)
    {
        if (!lista[i].isSet())
        {
            Lector *lector = Lector::getInstance();
            lista[i].setValor(lector->leerConfig(lista[i].getTag()));
        }
    }
}

string Paramlist::getValor(string arg)
{
    int pos = buscarParametro(arg);
    if (pos == -1)
    {
        return "";
    }
    return lista.at(pos).getValor();
}

Paramlist *Paramlist::getInstance()
{
    if (paramlist == nullptr)
    {
        paramlist = new Paramlist();
    }
    return paramlist;
}

Paramlist *Paramlist::getInstance(int argc, char **argv)
{
    if (paramlist == nullptr)
    {
        paramlist = new Paramlist(argc, argv);
    }
    return paramlist;
}