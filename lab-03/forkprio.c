#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>      // times()
#include <sys/resource.h>
#include <string.h>
#include <signal.h>

const int MAX_HIJOS = 10; // cantidad maxima de hijos
const int MAX_TIEMPO = 30;  // cantidad maxima de sleep

int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

void childmessage(int sig){    // recibida la senal de kill, imprimo un mensaje
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    //printf("%d : %s\n", sig, strsignal(sig)); // imprimo info senal
    // imprimo info hijo
    printf("Child %d (nice %2d):\t%3li\n",
    (int) getpid(),
    getpriority(PRIO_PROCESS,(id_t) getpid()),
    usage.ru_utime.tv_sec + usage.ru_stime.tv_sec);
    exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[])
{
    int i,pid,
    nice_i=0;
    int hijos[MAX_HIJOS];

    if(atoi(argv[1]) > 0 && atoi(argv[1]) <= MAX_HIJOS 
        && atoi(argv[2]) > 0 && atoi(argv[2]) <= MAX_TIEMPO) {     // verifico si cant hijos y tiempo son validos

        for(i=0 ; i < atoi(argv[1]) ; i = i + 1) {   // creo la cantidad de hijos indicada
            pid = fork();   // creo un hijo
            hijos[i] = pid;

            if (pid == 0) { // si soy hijo, salgo del for
                signal(SIGTERM, childmessage);  // si las sig senales, llamo al manejador
                signal(SIGINT, childmessage);

                if (atoi(argv[3]) == 1){
                    nice(nice_i);    // asigno la prioridad del hijo
                }
                
                break;
            }
            nice_i = nice_i + 1;
        }

        if (pid == 0) { // si soy proceso hijo,
            printf("hijo %d, prioridad %2d \n", getpid(), 
            getpriority(PRIO_PROCESS,(id_t)getpid()));

            busywork();
        } else {

            if (atoi(argv[2]) > 0) { 
                sleep((unsigned int) atoi(argv[2]));    // duermo por el tiempo asignado

                for (i = 0 ; i < atoi(argv[2]) ; i = i + 1){
                    kill(hijos[i], SIGINT);
                }
            } else if (atoi(argv[2]) == 0) {    // si tiempo es 0, espero a que yo mande la senal
                pause();
            }
        }
    } else {
        printf("Parametros invalidos.");
    }
    
    exit(EXIT_SUCCESS);
}
