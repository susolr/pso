/**
 * @file pso.cpp
 * @author Jesús López Rodríguez (jlopezpeque@hotmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "pso.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "lector.h"
#include <omp.h>
#include "paramlist.h"
#include <mpi.h>

using namespace std;

PSO::PSO(){
    Lector * lector = Lector::getInstance();
    lector->setDataBase("104");
    lector->leerDatos(178,3600);
    dimension = 3600;
    //lector->mostrarDatos();
    b_value = 0.0;
}


//MASTER

void PSO::crearCumulo(){
    //cout << "Creando cumulo" << endl;
    cumulo.clear();
    int n_particulas = stoi(Paramlist::getInstance()->getValor("-nP"));
    for (int i = 0; i < n_particulas; i++){
        cumulo.push_back(Particula(dimension));
        //cout << "Creada partícula " << i << endl;
    }
    //cout << "Tamaño del cúmulo: " << cumulo.size() << endl;
    b_pos = cumulo[0].getPos();
}

void PSO::ejecutar(){
    //cout << "Bucle principal" << endl;
    int n_iter = 0;
    int contador = 0;
    int n_threads = stoi(Paramlist::getInstance()->getValor("-nH"));
    int n_max_iter = stoi(Paramlist::getInstance()->getValor("-nI"));

    MPI::Status status;
	MPI::Datatype array_of_types[3] = {MPI::UNSIGNED_CHAR, MPI::FLOAT, MPI::INT};

	// The 'Individual' datatype must be converted to a MPI datatype and commit it
	MPI::Aint array_of_displacement[3] = {offsetof(Individual, chromosome), offsetof(Individual, fitness), offsetof(Individual, rank)};
	MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(3, array_of_blocklengths, array_of_displacement, array_of_types);
	Particle_MPI_type.Commit();

    int mpiSize = stoi(Paramlist::getInstance()->getValor("-size"));

    MPI::Request requests[conf -> mpiSize - 1];

    MPI::COMM_WORLD.Barrier();
    
    while (contador < n_max_iter){
        //cout << "Iter: " << contador << endl;
        double aux_value = b_value, var_value;
        //int max = 0;
        double clas_media = 0.0;
        //cout << "Particula 0: " << cumulo[0].getPos()[2] << endl;
        //cout << "Valorando..." << endl;

        #pragma omp parallel num_threads(n_threads)
        {

            if (mpiSize == 1){ //Trabaja únicamente la máquina master
                #pragma omp for
                    for (int i = 0; i < cumulo.size(); i++){
                        cumulo[i].valorar();     
                    }
            }
            else { //Trabajan los workers

                //envío el trabajo
                #pragma omp for
                    for (int i = 0; i < cumulo.size(); i++){
                        //MPI::COMM_WORLD.Isend(NULL, 0, MPI::INT, p, FINISH);
                    }
                //Trabajo???
                /*


                */

                //recojo resultados
                #pragma omp for
                    for (int i = 0; i < cumulo.size(); i++){
                        MPI::COMM_WORLD.Ireceive(NULL, 0, MPI::INT, p, FINISH);
                    }

            }
            
            
            //cout << "Antes de actualizacion b_pos" << endl;
            #pragma omp master
            {
                bool cambia = false;
                int pos;
                for (int i = 0; i < cumulo.size(); i++){
                    if (cumulo[i].getBValue() > b_value){
                        cambia = true;
                        b_value = cumulo[i].getBValue();
                        pos = i;
                    }
                    clas_media += cumulo[i].getValue();
                }
                if (cambia){
                        b_pos.clear();
                        b_pos = cumulo[pos].getBPos();
                        //b_k = cumulo[pos].getBK();
                }
            }
            
            //cout << "Valoradas todas las partículas" << endl;
            //cout << "Actualizando velocidad y posicion..." << endl;
            #pragma omp for 
                for (int i = 0; i < cumulo.size(); i++){
                    //cout << "Actualizando: " << i << endl;
                    //cout << "Tamaño del cúmulo " << cumulo.size() << endl;
                    //cout << "Tamaño de b_pos " << b_pos.size() << endl;
                    cumulo[i].actualizarVelocidad(b_pos);
                    cumulo[i].actualizarPosicion();
                }
        }
        //cout << "Velocidad y posicion actualizadas" << endl;
        var_value = abs(b_value-aux_value);
        clas_media = clas_media/cumulo.size();
        cout << "0," << clas_media << "\t0," << b_value << endl;
        /*if (var_value < 0.001){
            ++n_iter;
        }
        else {
            n_iter = 0;
        }*/
        contador++;
        
    }

    for (int p = 1; p < conf -> mpiSize; ++p) {
        requests[p - 1] = MPI::COMM_WORLD.Isend(NULL, 0, MPI::INT, p, FINISH);
	}
}

void PSO::mostrarResultados(){
    cout << "Monstrando resultados "  << endl;
    cout << "RESULTADOS:" << endl;
    cout << "Mejor valor: " << b_value << "%" << endl;
    //cout << "Mejor k: " << b_k << endl;
    //cout << "Mejor posicion:";
    /*for (int i = 0; i < b_pos.size(); i++){
        cout << "\t" << b_pos[i];
    }*/
    cout << endl;

}

void PSO::valorar(){
    MPI::Status status;
	MPI::Datatype array_of_types[3] = {MPI::UNSIGNED_CHAR, MPI::FLOAT, MPI::INT};

	// The 'Individual' datatype must be converted to a MPI datatype and commit it
	MPI::Aint array_of_displacement[3] = {offsetof(Individual, chromosome), offsetof(Individual, fitness), offsetof(Individual, rank)};
	MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(3, array_of_blocklengths, array_of_displacement, array_of_types);
	Particle_MPI_type.Commit();

    int mpiSize = stoi(Paramlist::getInstance()->getValor("-size"));

    MPI::COMM_WORLD.Barrier();

    do {

    } while (true);
}

void PSO::crearParticula(){
    

    particula = Particula(dimension);
}


// WORKERS