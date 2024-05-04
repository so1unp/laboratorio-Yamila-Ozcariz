#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

const int MAX_THREADS = 20;
const int MAX_SLEEP = 10;

void *tsleep (void *count){
    int trsleep = (rand() % atoi(count)) + 1;
    int* amount = malloc(sizeof(int));
    *amount = trsleep;

    printf("Hilo %u: dormire %d\n",
    (unsigned int) pthread_self(), trsleep);

    sleep(trsleep);
    pthread_exit((void *) amount);
}

int main(int argc, char* argv[])
{
    int i,pcreate;
    int* psleep;
    pthread_t tid[MAX_THREADS];

    if(argc != 3 && argv[1] <= 0 && argv[2] <=0) {
        //verifico argumentos
        printf("Argumentos invalidos.\n");
        exit(0);
    }

    for (i = 0 ; i < atoi(argv[1]) ; i = i + 1) {
        pcreate = pthread_create(&tid[i],NULL,*tsleep,argv[2]);

        if( pcreate != 0){
            perror("pthread_create");
            exit(0);
        } 
    }
    
    for (i = 0 ; i < atoi(argv[1]) ; i = i + 1) {
        pthread_join(tid[i],(void **) &psleep);
        printf("Hilo %u termino: %d\n",
        (unsigned int) tid[i], *psleep);
    }

    free(psleep);
    exit(EXIT_SUCCESS);
    //pthread_exit(NULL);
    //return 0;
}

