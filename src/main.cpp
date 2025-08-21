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
 * @file main.cpp
 * @author Juan José Escobar Pérez and Jesús López Rodríguez
 * @date 02/02/2024
 * @brief A parallel and distributed Binary Particle Swarm Optimization (BPSO) algorithm to EEG
 * classification
 * @copyright Atom (c) 2024 University of Granada
 */

#include <mpi.h>
#include <signal.h>     // Para la función kill
#include <sys/types.h>  // Para el tipo pid_t
#include <unistd.h>     // Para la función getpid, fork

#include <cmath>
#include <cstdlib>  // Para la función system
#include <ctime>
#include <iostream>
#include <queue>

#include "lector.h"
#include "omp.h"
#include "paramlist.h"
#include "pso.h"
#include "webSocketServer.h"
#include "pso_manager.h"
#include <thread>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

    // printf("Hybrid: Hello from process %d/%d on %s\n", rank, numprocs, processor_name);

    // MPI::Init_thread(MPI_THREAD_MULTIPLE);

    Paramlist *lista;
    if (argc == 1) {
        lista = Paramlist::getInstance();
    } else {
        lista = Paramlist::getInstance(argc, argv);
    }

    // cout << "N hebras" << lista->getValor("-nH") << endl;

    srand((uint)time(NULL) + stoi(lista->getValor("MPIrank")));

    PSO mi_pso = PSO();
    double time_inicio;
    double time;

    if (stoi(lista->getValor("MPIrank")) == 0) {
        // cout << "Master tamanio: " << lista->getValor("MPIsize") << endl << flush;
        auto &ws = WebSocketServer::getInstance();
        int wsc = stoi(lista->getValor("-WSC"));
        if (wsc) {
            ws.start(9999);
            // Esperar órdenes por WebSocket (start/scriptRun)
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        } else {
            // Modo standalone: ejecutar una sola vez y salir
            PSOManager::getInstance().startSingle();
        }
    } else {
        // cout << "Worker: " << lista->getValor("MPIrank") << endl << flush;
        mi_pso.valorar();
    }

    MPI::Finalize();

    return 0;
}