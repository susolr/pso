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
        int n;
        int pos[3];
} part;

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
    int          blocklengths[2] = {1,3};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_particula_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(part, n);
    offsets[1] = offsetof(part, pos);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_particula_type);
    MPI_Type_commit(&mpi_particula_type);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        part * send = new part[2];
        vector<int> vec;
        vec.push_back(4);
        vec.push_back(100);
        vec.push_back(250);
        send[0].n = 0;

        send[0].pos[0] = vec.at(0);
        send[0].pos[1] = vec.at(1);
        send[0].pos[2] = vec.at(2);

        send[1].n = 1;

        send[1].pos[0] = vec.at(0)*10;
        send[1].pos[1] = vec.at(1)*10;
        send[1].pos[2] = vec.at(2)*10;

        cout << "Rank " << rank << endl << flush;
        cout << "n = " << send[0].n << endl << flush;
        cout << "Sent: pos[0] = " << send[0].pos[0] << endl << flush;
        cout << "Sent: pos[1] = " << send[0].pos[1] << endl << flush;
        cout << "Sent: pos[2] = " << send[0].pos[2] << endl << flush;
        cout << "n = " << send[1].n << endl << flush;
        cout << "Sent: pos[0] = " << send[1].pos[0] << endl << flush;
        cout << "Sent: pos[1] = " << send[1].pos[1] << endl << flush;
        cout << "Sent: pos[2] = " << send[1].pos[2] << endl << flush;
        const int dest = 1;
        MPI_Send(send,   2, mpi_particula_type, dest, tag, MPI_COMM_WORLD);

        
    }
    if (rank == 1) {
        MPI_Status status;
        const int src=0;
        part * recv = new part[2];

        MPI_Recv(recv,   2, mpi_particula_type, src, tag, MPI_COMM_WORLD, &status);
        cout << "Rank " << rank << endl << flush;
        cout << "n = " << recv[0].n << endl << flush;
        cout << "Received: pos[0] = " << recv[0].pos[0] << endl << flush;
        cout << "Received: pos[1] = " << recv[0].pos[1] << endl << flush;
        cout << "Received: pos[2] = " << recv[0].pos[2] << endl << flush;
        cout << "n = " << recv[1].n << endl << flush;
        cout << "Received: pos[0] = " << recv[1].pos[0] << endl << flush;
        cout << "Received: pos[1] = " << recv[1].pos[1] << endl << flush;
        cout << "Received: pos[2] = " << recv[1].pos[2] << endl << flush;
    
    }

    MPI_Type_free(&mpi_particula_type);
    MPI_Finalize();



    return 0;
}