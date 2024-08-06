#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <ctime>

#define cols 20000
#define rows 20000
#define v(r, c) ((r) * cols + (c)) // Efficient matrix indexing

void fill_matrix(int *matrice) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrice[v(i, j)] = rand() % 101; // Random value between 0 and 100
        }
    }
    int randomRow = rand() % rows;
    int randomCol = rand() % cols;
    matrice[v(randomRow, randomCol)] = 666;
}

int main(int argc, char** argv) {
    int my_rank;
    int nproc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    const int chunk_size = rows / nproc; // Calculate chunk size for each process

    if (my_rank == 0) { // Process 0 (master)
        int *matrice = new int[cols * rows];
        fill_matrix(matrice);

        int *massimi = new int[nproc]; // Allocate memory for massimi

        // Scatter the matrix in chunks to slave processes
        for (int i = 1; i < nproc; ++i) {
            MPI_Send(matrice + v(chunk_size * i, 0), chunk_size * cols, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        clock_t start = clock();
        // Find the maximum value in the master's chunk
        int maxVal = -1;
        for (int i = 0; i < chunk_size; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (matrice[v(i, j)] > maxVal) {
                    maxVal = matrice[v(i, j)];
                }
            }
        }
        massimi[0] = maxVal;

        // Receive maximum values from slave processes
        for (int i = 1; i < nproc; ++i) {
            MPI_Recv(&massimi[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        printf("Received maximum values from slaves: ");
        for (int i = 0; i < nproc; i++) {
            printf("%d ", massimi[i]);
        }
        printf("\n");

        int tempmax = massimi[0];
        for (int i = 1; i < nproc; i++) {
            if (tempmax < massimi[i]) {
                tempmax = massimi[i];
            }
        }
        clock_t end = clock();
        double duration = (double)(end-start)/ CLOCKS_PER_SEC*1000.0;
        printf("The overall maximum value is: %d\n", tempmax);
        printf("durata:%f\n",duration);

        delete[] matrice;
        delete[] massimi;
    } else { // Slave processes (rank > 0)
        int *submatrix = new int[chunk_size * cols];
        MPI_Recv(submatrix, chunk_size * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int maxVal = -1; // Initialize with a sufficiently low value
        for (int i = 0; i < chunk_size; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (submatrix[i * cols + j] > maxVal) {
                    maxVal = submatrix[i * cols + j]; // Corrected indexing within submatrix
                }
            }
        }
        MPI_Send(&maxVal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        delete[] submatrix;
    }

    MPI_Finalize();

    return 0;
}
