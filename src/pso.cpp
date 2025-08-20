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
 * @file pso.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief Implementation of the BPSO algorithm
 * @copyright Atom (c) 2024 University of Granada
 */

#include "pso.h"

#include <mpi.h>
#include <omp.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "lector.h"
#include "paramlist.h"
#include "webSocketServer.h"

/********Defines********/
#define IGNORE_VALUE 1
#define FINISH -1

using json = nlohmann::json;
using namespace std;

PSO::PSO() {
    Lector *lector = Lector::getInstance();
    lector->setDataBase("001");
    dimension = N_FEATURES;
    lector->leerDatos(288, dimension);

    // lector->mostrarDatos();
    b_value = 0.0;
}

// MASTER

void PSO::crearCumulo() {
    // cout << "Creando cumulo" << endl;
    cumulo.clear();
    int n_particles = stoi(Paramlist::getInstance()->getValor("-nP"));
    for (int i = 0; i < n_particles; i++) {
        cumulo.push_back(Particula(dimension));
        // cout << "Creada partícula " << i << endl;
    }
    // cout << "Tamaño del cúmulo: " << cumulo.size() << endl;
    b_pos = cumulo[0].getPos();
}

// Workers
void PSO::crearCumulo(int n_p) {
    // cout << "Creando cumulo" << endl;
    cumulo.clear();
    for (int i = 0; i < n_p; i++) {
        cumulo.push_back(Particula(dimension));
        // cout << "Creada partícula " << i << endl;
    }
    // cout << "Tamaño del cúmulo: " << cumulo.size() << endl;
    b_pos = cumulo[0].getPos();
}

void PSO::ejecutar() {
    // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" entra al bucle
    // principal" << endl << flush;
    auto &ws = WebSocketServer::getInstance();
    int n_iter = 0;
    int n_threads = stoi(Paramlist::getInstance()->getValor("-nH"));
    // cout << "num_threads " << n_threads << endl << flush;
    int n_max_iter = stoi(Paramlist::getInstance()->getValor("-nI"));
    omp_set_num_threads(n_threads);
    MPI::Status status;
    MPI::Datatype array_of_types[3] = {MPI::INT, MPI::DOUBLE, MPI::INT};
    int array_of_blocklengths[3] = {1, 1, dimension};

    // The 'Individual' datatype must be converted to a MPI datatype and commit it
    MPI::Aint array_of_displacement[3] = {offsetof(particula_mpi, n_particula),
                                          offsetof(particula_mpi, valor),
                                          offsetof(particula_mpi, pos)};
    MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(
        3, array_of_blocklengths, array_of_displacement, array_of_types);
    Particle_MPI_type.Commit();

    int mpiSize = stoi(Paramlist::getInstance()->getValor("MPIsize"));

    int *sizes = new int[mpiSize - 1];

    if (mpiSize > 1) {
        int n_particles = stoi(Paramlist::getInstance()->getValor("-nP"));
        int tam = n_particles / (mpiSize - 1);

        // cout << "Tengo q distribuir" << endl << flush;
        int rest = n_particles % (mpiSize - 1);
        for (int i = 1; i < mpiSize; i++) {
            sizes[i - 1] = (i <= rest) ? tam + 1 : tam;
        }

        // sizes.push_back(rest);

        /*for (int i = 0; i < mpiSize - 1; i++)
        {
            int dest = i + 1;
            int envio = sizes.at(i);
            // cout << "Envio el tamanio del problema: " << sizes.at(i) << endl << flush;
            MPI::COMM_WORLD.Send(&envio, 1, MPI::INT, dest, IGNORE_VALUE);
            // cout << "Tamanio enviado" << endl << flush;
        }*/
    }

    // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" Antes de
    // barrier" << endl << flush;

    // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" despues de
    // barrier" << endl << flush;

    for (int contador = 0; contador < n_max_iter; ++contador) {
        double iterationInitTime = omp_get_wtime();
        // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" entra a
        // valorar" << endl << flush; cout << "Iter: " << contador << endl;
        double aux_value = b_value, var_value;
        // int max = 0;
        double clas_media = 0.0;
        // cout << "Particula 0: " << cumulo[0].getPos()[2] << endl;
        // cout << "Valorando..." << endl;

        if (mpiSize == 1) {  // Trabaja únicamente la máquina master
#pragma omp parallel for
            for (int i = 0; i < cumulo.size(); i++) {
                // cout << "Iteracion " << contador << "Particula: " << i << " valorada por hebra "
                // << omp_get_thread_num() << endl << flush;
                cumulo[i].valorar();
            }
        } else {  // Trabajan los workers
            // envío el trabajo

            int cont_aux = sizes[0];
            for (int i = 0; i < mpiSize - 1; i++) {
                particula_mpi *aux = new particula_mpi[sizes[i]];
                int tm = sizes[i];
                for (int j = 0; j < tm; j++) {
                    int index = i * cont_aux + j;
                    aux[j] = cumulo[index].toStruct();
                    aux[j].n_particula = index;
                }
                // cout << "Envio grupo de particulas" << endl << flush;
                // Cambiar por ISend
                MPI::COMM_WORLD.Send(aux, tm, Particle_MPI_type, i + 1, IGNORE_VALUE);
                // cout << "Grupo enviado" << endl << flush;

                delete[] aux;
            }

            // recojo resultados

            for (int i = 0; i < mpiSize; i++) {
                particula_mpi *aux = new particula_mpi[sizes[i]];
                int tm = sizes[i];
                int rec = i + 1;
                MPI::COMM_WORLD.Recv(aux, tm, Particle_MPI_type, rec, MPI::ANY_TAG, status);
                for (int j = 0; j < tm; j++) {
                    int index = i * cont_aux + j;
                    cumulo[index].setValue(aux[j].valor);
                }
                delete[] aux;
            }
        }

        // cout << "Antes de actualizacion b_pos" << endl;

        bool cambia = false;
        int pos;
        for (int i = 0; i < cumulo.size(); i++) {
            if (cumulo[i].getBValue() > b_value) {
                cambia = true;
                b_value = cumulo[i].getBValue();
                pos = i;
            }
            clas_media += cumulo[i].getValue();
        }
        if (cambia) {
            b_pos.clear();
            b_pos = cumulo[pos].getBPos();
            // b_k = cumulo[pos].getBK();
        }

// cout << "Valoradas todas las partículas" << endl;
// cout << "Actualizando velocidad y posicion..." << endl;
#pragma omp parallel for
        for (int i = 0; i < cumulo.size(); i++) {
            // cout << "Actualizando: " << i << endl;
            // cout << "Tamaño del cúmulo " << cumulo.size() << endl;
            // cout << "Tamaño de b_pos " << b_pos.size() << endl;
            cumulo[i].actualizarVelocidad(b_pos);
            cumulo[i].actualizarPosicion();
        }

        // cout << "Velocidad y posicion actualizadas" << endl;
        // var_value = abs(b_value-aux_value);
        clas_media = clas_media / cumulo.size();
        // cout << "0," << clas_media << "\t0," << b_value << "\t" << b_k << endl;
        cout << "0," << clas_media << "\t0," << b_value << endl;
        double iterationTime = omp_get_wtime() - iterationInitTime;
        std::ostringstream envio;
        envio << clas_media << "\t" << b_value << endl;
        json telemtry = {
            {"type", "telemetry"},
            {"payload", {{"iteration", contador}, {"average", clas_media}, {"best", b_value}, {"executionTime", iterationTime}, {"position", b_pos}}}};

        ws.broadcast(telemtry.dump());
    }

    for (int p = 1; p < mpiSize; ++p) {
        MPI::COMM_WORLD.Send(NULL, 0, Particle_MPI_type, p, FINISH);
    }

    MPI::COMM_WORLD.Barrier();
    Particle_MPI_type.Free();

    cout << endl;
    for (int i = 0; i < b_pos.size(); i++) {
        if (b_pos.at(i) == 1) {
            cout << i << endl;
        }
    }
    cout << endl;
}

void PSO::valorar() {
    // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" entra a
    // valorar" << endl << flush;
    MPI::Status status;
    MPI::Datatype array_of_types[3] = {MPI_INT, MPI_DOUBLE, MPI_INT};
    int array_of_blocklengths[3] = {1, 1, dimension};

    // The 'Individual' datatype must be converted to a MPI datatype and commit it
    MPI::Aint array_of_displacement[3] = {offsetof(particula_mpi, n_particula),
                                          offsetof(particula_mpi, valor),
                                          offsetof(particula_mpi, pos)};
    MPI::Datatype Particle_MPI_type = MPI::Datatype::Create_struct(
        3, array_of_blocklengths, array_of_displacement, array_of_types);
    Particle_MPI_type.Commit();

    int mpiSize = stoi(Paramlist::getInstance()->getValor("MPIsize"));

    // MPI::COMM_WORLD.Barrier();

    int tam;
    // cout << "Recibo el tamanio del problema" << endl << flush;
    // MPI::COMM_WORLD.Recv(&tam, 1, MPI::INT, 0, MPI::ANY_TAG, status);
    // cout << "Tamanio recibido" << endl << flush;

    particula_mpi *particulas = new particula_mpi[tam];

    crearCumulo(tam);
    // cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" Antes de
    // barrier" << endl << flush; MPI::COMM_WORLD.Barrier(); cout << "Proceso: " <<
    // stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" Despues de barrier" << endl << flush;
    // Comienza el bucle principal

    // cout << "Recibo el primer grupo de particulas" << endl << flush;
    MPI::COMM_WORLD.Recv(particulas, tam, Particle_MPI_type, 0, MPI::ANY_TAG, status);
    // cout << "Primer grupo recibido" << endl << flush;

    while (status.Get_tag() != FINISH) {
// cout << "Proceso: " << stoi(Paramlist::getInstance()->getValor("MPIrank")) <<" entra a valorar"
// << endl << flush;
#pragma omp parallel for
        for (int i = 0; i < tam; i++) {
            cumulo[i].fromStruct(particulas[i]);
            cumulo[i].valorar();
            particulas[i].valor = cumulo[i].getValue();
        }
        MPI::COMM_WORLD.Send(particulas, tam, Particle_MPI_type, 0, IGNORE_VALUE);
        MPI::COMM_WORLD.Recv(particulas, tam, Particle_MPI_type, 0, MPI::ANY_TAG, status);
    }

    MPI::COMM_WORLD.Barrier();

    Particle_MPI_type.Free();

    delete[] particulas;
}

void PSO::mostrarResultados() {
    cout << "Monstrando resultados " << endl;
    cout << "RESULTADOS:" << endl;
    cout << "Mejor valor: " << b_value << "%" << endl;
    // cout << "Mejor k: " << b_k << endl;
    // cout << "Mejor posicion:";
    /*for (int i = 0; i < b_pos.size(); i++){
        cout << "\t" << b_pos[i];
    }*/
    cout << endl;
}

void PSO::infoHybrid() {
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int iam = 0, np = 1;

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

// omp_set_num_threads(omp_get_max_threads());
#pragma omp parallel default(shared) private(iam, np)
    {
        np = omp_get_num_threads();
        iam = omp_get_thread_num();
#pragma omp critical
        {
            printf("Hybrid: Hello from thread %d/%d from process %d/%d on %s\n", iam, np, rank,
                   numprocs, processor_name);
        }
    }
}

// WORKERS