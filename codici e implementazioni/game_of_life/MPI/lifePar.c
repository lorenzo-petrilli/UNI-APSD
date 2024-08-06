/**********************************************************************
 * life in parallel
 *
 **********************************************************************/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NCOLS 20
#define NROWS 30

#define v(r,c) ((r)*NCOLS+(c))

int* readM;
int* writeM;
int nsteps=150;


int rank, rankUp, rankDown;
int nProc;


void init(){


	for(int i=0;i<NROWS/nProc+2;i++)
		for(int j=0;j<NCOLS;j++)
			readM[v(i,j)]=0;


	if (rank==0){
		int ii = 3;
		int jj = NCOLS/2;
		readM[v(ii-1,jj)]=1;
		readM[v(ii,jj+1)]=1;
		readM[v(ii+1,jj-1)]=1;
		readM[v(ii+1,jj)]=1;
		readM[v(ii+1,jj+1)]=1;
	}
}

void print(int step) {
    if (rank == 0) {
	//printf("sono 0 nella print\n");
        int* totalM = new int[NCOLS * NROWS];
        MPI_Status status;

        for (int i = 0; i < (NROWS/nProc); i++) {
            for (int j = 0; j < NCOLS; j++) {
                totalM[v(i, j)] = readM[v(i + 1, j)];
            }
        }

        for (int core = 1; core < nProc; core++) {
            MPI_Recv(&totalM[core * (NROWS / nProc) * NCOLS], (NROWS / nProc) * NCOLS, MPI_INT, core, 10, MPI_COMM_WORLD, &status);
        }

        printf("--- %d ---\n", step);
        for (int i = 0; i < NROWS; i++) {
            for (int j = 0; j < NCOLS; j++) {
                char ch = '0';
                if (totalM[v(i, j)] == 1)
                    ch = '*';
                printf("%c ", ch);
            }
            printf("\n");
        }
        printf("end print\n");
        delete[] totalM;
    } else {
	//printf("sono slave nella print\n");
        MPI_Send(&readM[v(1, 0)], (NROWS / nProc) * NCOLS, MPI_INT, 0, 10, MPI_COMM_WORLD);
    }
}



void swap(){
	int * p=readM;
	readM=writeM;
	writeM=p;
}


inline void exchBord(){
	MPI_Request request;
	MPI_Status status;
	int otherNode=0;
	// south 17
	// north 20
	if (rank == 0)
		otherNode=1;
	MPI_Isend(&readM[v(NROWS/nProc,0)], NCOLS, MPI_INT, rankDown, 17, MPI_COMM_WORLD, &request);
	MPI_Isend(&readM[v(1,0)], NCOLS, MPI_INT, rankUp, 20, MPI_COMM_WORLD, &request);
	MPI_Recv(&readM[v(NROWS/nProc+1,0)], NCOLS, MPI_INT, rankDown, 20, MPI_COMM_WORLD, &status);
	MPI_Recv(&readM[v(0,0)], NCOLS, MPI_INT, rankUp, 17, MPI_COMM_WORLD, &status);


}

inline void transFuncCell(int i, int j){
	int cont=0;
	for(int di=-1;di<2;di++)
		for(int dj=-1;dj<2;dj++)
			if ((di!=0 || dj!=0) &&readM[v((i+di+NROWS)%NROWS,(j+dj+NCOLS)%NCOLS)]==1)
				cont++;

	if (readM[v(i,j)]==1)
		if (cont==2 || cont ==3)
			writeM[v(i,j)]=1;
		else
			writeM[v(i,j)]=0;
	else
		if (cont ==3)
			writeM[v(i,j)]=1;
		else
		writeM[v(i,j)]=0;
}


inline void transFunc(){

	for(int i=1;i<NROWS/nProc+1;i++){
		for(int j=0;j<NCOLS;j++){
			transFuncCell(i,j);

		}

	}
}

int main(int argc, char *argv[]) {



	MPI_Init(&argc, &argv);               /* Initialize MPI               */
	MPI_Comm_size(MPI_COMM_WORLD, &nProc); /* Get the number of processors */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* Get my number                */

	rankUp = (rank-1+nProc)%nProc;
	rankDown = (rank+1)%nProc;

	readM = new int[(NROWS/nProc+2)*NCOLS];
	writeM = new int[(NROWS/nProc+2)*NCOLS];



	init();

	double starttime = MPI_Wtime();

	for(int s=0;s<nsteps;s++){
		exchBord();
		print(s);
		transFunc();
		swap();
	}

	double endtime = MPI_Wtime();

	if (rank == 0){
		printf("Elapsed time: %f\n", 1000*(endtime - starttime));
	}

	delete[] readM;
	delete[] writeM;

	MPI_Finalize();


	return 0;
}
