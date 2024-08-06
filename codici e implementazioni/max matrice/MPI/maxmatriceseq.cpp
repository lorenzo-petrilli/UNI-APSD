
#include <iostream>
#include <cstdlib>
#include <ctime>

#define NCOLS 20000
#define NROWS 20000

#define v(r,c) ((r)*NCOLS+(c))

int main() {
    // Allocazione dinamica della matrice
    int* matrix = new int[NROWS * NCOLS];

    // Inizializzazione del generatore di numeri casuali
    srand(time(0));

    // Inizializza la matrice con valori casuali tra 0 e 100
    for(int i = 0; i < NROWS; ++i) {
        for(int j = 0; j < NCOLS; ++j) {
            matrix[v(i, j)] = rand() % 101; // Valore casuale tra 0 e 100
        }
    }

    // Assegna il valore 666 a un elemento a caso
    int randomRow = rand() % NROWS;
    int randomCol = rand() % NCOLS;
    matrix[v(randomRow, randomCol)] = 666;

    

    // Misura il tempo per trovare il massimo valore nella matrice
    clock_t start = clock();
    int maxVal = 0;
    for(int i = 0; i < NROWS; ++i) {
        for(int j = 0; j < NCOLS; ++j) {
            if (matrix[v(i, j)] > maxVal) {
                maxVal = matrix[v(i, j)];
            }
        }
    }
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000.0; // Converti in millisecondi

    // Stampa il massimo valore trovato e il tempo impiegato
    std::cout << "Il massimo valore nella matrice è: " << maxVal << std::endl;
    std::cout << "Tempo impiegato per calcolare il massimo: " << duration << " millisecondi" << std::endl;

    // Deallocazione della memoria
    delete[] matrix;

    return 0;
}
