#include <iostream>
#include <cstdlib>
#include <ctime>
#include <time.h>
using namespace std;

#define step 2000000000


int main(){
        srand(time(NULL));
        int count_circle=0;
        int count_square=0;
        clock_t start = clock();
        for (int i=0;i<step;i++){
                double x = static_cast<double>(rand()) / RAND_MAX;
                double y = static_cast<double>(rand()) / RAND_MAX;
                if((x*x)+(y*y)<=1.0){
                        count_circle++;
                }

        }
        clock_t end = clock();
        double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000.0; // Converti in millisecondi
        double pi_estimate = 4.0 * static_cast<double>(count_circle) / step;
        cout << "Estimated Pi: " << pi_estimate << endl;
        cout<<"tempo trascorso: "<<duration<<endl;
}