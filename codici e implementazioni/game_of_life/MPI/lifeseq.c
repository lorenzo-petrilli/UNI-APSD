
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctime>  // Per la misurazione del tempo

#define NCOLS 20
#define NROWS 30

#define v(r,c) ((r)*NCOLS+(c)) // linearizzazione dello spazio

// Gioco della Vita - Sequenziale
int* readM = new int[NROWS*NCOLS];
int* writeM = new int[NROWS*NCOLS];
int nsteps=150;


void init(){
	int ii = NROWS/2;
	int jj = NCOLS/2;

	for(int i=0;i<NROWS;i++)
		for(int j=0;j<NCOLS;j++)
			readM[v(i,j)]=0;

	// Aliante - Glider
	readM[v(ii-1,jj)]=1;
	readM[v(ii,jj+1)]=1;
	readM[v(ii+1,jj-1)]=1;
	readM[v(ii+1,jj)]=1;
	readM[v(ii+1,jj+1)]=1;
}

void print(int step){
	printf("---%d\n",step);
	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
			char ch = '0';
			if (readM[v(i,j)]==1)
				ch='*';
			printf("%c ",ch);

		}
		printf("\n");
	}

}

void swap(){
	int * p=readM;
	readM=writeM;
	writeM=p;
}

void transFuncCell(int i, int j){
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

void transFunc(){

	for(int i=0;i<NROWS;i++){
		for(int j=0;j<NCOLS;j++){
			transFuncCell(i,j);
		}
	}
}

int main(int argc, char* argv[]) {
    init();

    clock_t start = clock();  // Inizio della misurazione del tempo

    for (int s = 0; s < nsteps; s++) {
        print(s);  // stampa stato
        transFunc();
        swap();
    }

    clock_t end = clock();  // Fine della misurazione del tempo
    double elapsed_time = double(end - start) / CLOCKS_PER_SEC*1000.0;  // Calcolo del tempo trascorso in secondi
    printf("Elapsed time: %f milliseconds\n", elapsed_time);  // Stampa del tempo trascorso

    delete[] readM;
    delete[] writeM;
}

