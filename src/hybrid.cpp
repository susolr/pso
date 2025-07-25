#include <mpi.h>
#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int iam = 0, np = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);

// omp_set_num_threads(omp_get_max_threads());
#pragma omp parallel default(shared) private(iam, np)
    {
        np = omp_get_num_threads();
        iam = omp_get_thread_num();
#pragma omp master
        {
            printf("Hybrid: Hello from thread %d/%d from process %d/%d on %s\n", iam, np, rank,
                   numprocs, processor_name);
        }
    }

    MPI_Finalize();

    return 0;
}