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

/********INCLUDES********/
#include "pso.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "lector.h"
#include <omp.h>
#include "paramlist.h"
#include <mpi.h>


/********Defines********/
#define IGNORE_VALUE 1
#define FINISH 2


/********NAMESPACE********/
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

//Workers
void PSO::crearCumulo( int n_p){
    //cout << "Creando cumulo" << endl;
    cumulo.clear();
    for (int i = 0; i < n_p; i++){
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
	MPI::Datatype array_of_types[3] = {MPI::INT, MPI::DOUBLE, MPI::INT};
    int array_of_blocklengths[3] = {1, 1, dimension};

	// The 'Individual' datatype must be converted to a MPI datatype and commit it
	MPI::Aint array_of_displacement[3] = {offsetof(particula_mpi, n_particula), offsetof(particula_mpi, valor), offsetof(particula_mpi, pos)};
	MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(3, array_of_blocklengths, array_of_displacement, array_of_types);
	Particle_MPI_type.Commit();


    int mpiSize = stoi(Paramlist::getInstance()->getValor("-size"));

    MPI::COMM_WORLD.Barrier();

    vector<int> tamanios;
    int tam = dimension/(mpiSize-1) + 1;
    int rest = dimension;
    if (mpiSize > 1){
        for (int i = 1; i < mpiSize - 1; i++){
            tamanios.push_back(tam);
            rest -= tam;
        }

        tamanios.push_back(rest);

        for (int i = 0; i < tamanios.size(); i++){
            int dest = i+1;
            int envio = tamanios.at(i);
            MPI::COMM_WORLD.Send(&envio, 1, MPI::INT, dest, IGNORE_VALUE);
        }

    }
    

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
                #pragma omp master
                {   
                    int cont_aux = tamanios.at(0);
                    for (int i = 0; i < tamanios.size(); i++){
                        particula_mpi * aux = new particula_mpi [tamanios.at(i)];
                        int tm = tamanios.at(i);
                        for(int j = 0; j < tm; j++){
                            int index = i*cont_aux + j;
                            aux[j] = cumulo[index].toStruct();
                            aux[j].n_particula = index;
                        }
                        MPI::COMM_WORLD.Isend(aux, tm, Particle_MPI_type, i, IGNORE_VALUE);

                        delete [] aux;
                    }

                //recojo resultados

                    for (int i = 0; i < tamanios.size(); i++){
                        particula_mpi * aux = new particula_mpi [tamanios.at(i)];
                        int tm = tamanios.at(i);
                        //MPI::COMM_WORLD.Isend(aux, tm, Particle_MPI_type, i, NONE);
                        int rec = i+1;
                        MPI::COMM_WORLD.Recv(aux, tm, Particle_MPI_type, rec, MPI::ANY_TAG, status);
                        for(int j = 0; j < tm; j++){
                            int index = i*cont_aux + j;
                            cumulo[index].setValue(aux[j].valor);
                        }
                        delete [] aux;
                    }
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
        contador++;
        
    }

    for (int p = 1; p < mpiSize; ++p) {
        MPI::COMM_WORLD.Send(NULL, 0, Particle_MPI_type, p, FINISH);
	}

    MPI::COMM_WORLD.Barrier();
    Particle_MPI_type.Free();
}


void PSO::valorar(){

    int n_threads = stoi(Paramlist::getInstance()->getValor("-nH"));
    MPI::Status status;
	MPI::Datatype array_of_types[3] = {MPI_INT, MPI_DOUBLE, MPI_INT};
    int array_of_blocklengths[3] = {1, 1, dimension};

	// The 'Individual' datatype must be converted to a MPI datatype and commit it
	MPI::Aint array_of_displacement[3] = {offsetof(particula_mpi, n_particula), offsetof(particula_mpi, valor), offsetof(particula_mpi, pos)};
	MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(3, array_of_blocklengths, array_of_displacement, array_of_types);
	Particle_MPI_type.Commit();

    int mpiSize = stoi(Paramlist::getInstance()->getValor("-size"));

    MPI::COMM_WORLD.Barrier();

    int tam;
    MPI::COMM_WORLD.Recv(&tam, 1, MPI::INT, 0, MPI::ANY_TAG, status);

    particula_mpi * particulas = new particula_mpi[tam];

    crearCumulo(tam);
    MPI::COMM_WORLD.Barrier();

    //Comienza el bucle principal
    MPI::COMM_WORLD.Recv(particulas, tam, Particle_MPI_type, 0, MPI::ANY_TAG, status);
    

    while (status.Get_tag() != FINISH){
        #pragma omp parallel for num_threads(n_threads)
            for (int i = 0; i < tam; i++){
                cumulo[i].fromStruct(particulas[i]);
                cumulo[i].valorar();
                particulas[i].valor = cumulo[i].getValue();
            }
        MPI::COMM_WORLD.Isend(particulas, tam, Particle_MPI_type, 0, IGNORE_VALUE);
        MPI::COMM_WORLD.Recv(particulas, tam, Particle_MPI_type, 0, MPI::ANY_TAG, status);
    }

    MPI::COMM_WORLD.Barrier();
    
    Particle_MPI_type.Free();

    delete [] particulas;

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


// WORKERS