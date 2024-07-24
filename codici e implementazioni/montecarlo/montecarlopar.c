#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

#define step 2000000000

int rank, nProc, mytotal;

void calcolo(int chunk) {
    int seed = time(NULL) + rank; // Different seed for each rank
    int count_circle = 0;
    for (int i = 0; i < chunk; i++) {
        double x = static_cast<double>(rand_r(&seed)) / RAND_MAX;
        double y = static_cast<double>(rand_r(&seed)) / RAND_MAX;
        if ((x * x) + (y * y) <= 1.0) {
            count_circle++;
        }
    }
    mytotal = count_circle;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);               // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &nProc); // Get the number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get my rank
    double starttime= MPI_Wtime();
    int chunk = step / nProc;
    calcolo(chunk);

    int total_count_circle = 0;

    if (rank == 0) {
        int count_slave = 0;
        MPI_Status status;
        for (int i = 1; i < nProc; i++) {
            int temp = 0;
            MPI_Recv(&temp, 1, MPI_INT, i, 20, MPI_COMM_WORLD, &status);
            count_slave += temp;
        }
        total_count_circle = mytotal + count_slave;
    } else {
        MPI_Send(&mytotal, 1, MPI_INT, 0, 20, MPI_COMM_WORLD);
    }
    double endtime=MPI_Wtime();

    if (rank == 0) {
        double pi_estimate = 4.0 * static_cast<double>(total_count_circle) / step;
        printf("Estimated Pi: %f\n", pi_estimate);
        printf("totale prove : %d\n",chunk);
        printf("totale countcerchio : %d\n",total_count_circle);
        printf("tempo trascorso:%f\n",1000*(endtime-starttime));
    }

    MPI_Finalize();
    return 0;
}
