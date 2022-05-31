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
#include <vector>
#include "lector.h"
#include "pso.h"
#include "omp.h"
#include "tinyxml2.h"
#include "paramlist.h"
#include <mpi.h>

using namespace std;
using namespace tinyxml2;

typedef struct car_s {
        int shifts;
        int topSpeed;
} car;

typedef struct particula_s {
        vector<int> pos (2, 0)  ;
        int n;
} particula;

int main (int argc, char** argv){

    const int tag = 13;
    int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr,"Requires at least two processes.\n");
        exit(-1);
    }

    /* create a type for struct car */
    const int nitems=2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_particula_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(particula, pos);
    offsets[1] = offsetof(particula, n);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_particula_type);
    MPI_Type_commit(&mpi_particula_type);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        particula send;
        send.pos.push_back(4);
        send.pos.push_back(100);
        send.n = 0;

        const int dest = 1;
        MPI_Send(&send,   1, mpi_particula_type, dest, tag, MPI_COMM_WORLD);

        printf("Rank %d: sent structure car\n", rank);
    }
    if (rank == 1) {
        MPI_Status status;
        const int src=0;

        particula recv;

        MPI_Recv(&recv,   1, mpi_particula_type, src, tag, MPI_COMM_WORLD, &status);
        printf("Rank %d: Received: pos[0] = %d pos[1] = %d n = %d\n", rank,
                 recv.pos[0], recv.pos[1], recv.n);
    }

    MPI_Type_free(&mpi_particula_type);
    MPI_Finalize();

    return 0;
}