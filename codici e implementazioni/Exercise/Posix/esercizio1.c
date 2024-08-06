#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int messaggi=0;
pthread_mutex_t mutex1;
void* routine(){
        for(int i=0;i<1000000;i++){
                pthread_mutex_lock(&mutex1);
                messaggi++;
                pthread_mutex_unlock(&mutex1);
        }
}

int main(int argc, char* argv[]){
        pthread_t t1,t2,t3;
        pthread_mutex_init(&mutex1,NULL);
        if (pthread_create(&t1,NULL,&routine,NULL)){
                return 1;
        };
        if (pthread_create(&t2,NULL,&routine,NULL)){
                return 2;
        };
        if (pthread_create(&t3,NULL,&routine,NULL)){
                return 3;
        };
        pthread_join(t1,NULL);
        pthread_join(t2,NULL);
        pthread_join(t3,NULL);
        pthread_mutex_destroy(&mutex1);
        printf("numero di messagi:%d\n",messaggi);
        return 0;
}