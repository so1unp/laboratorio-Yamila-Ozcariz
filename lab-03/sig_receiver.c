#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

void sighandler (int sig){
    printf("%d : %s\n", sig, strsignal(sig));
}

int main(void) {
    int signum;

    printf("PID: %d\n", getpid());
    
    for (signum = 0 ; signum < 255 ; signum = signum + 1) {
        signal (signum, sighandler);
    }

    while(1) {
        pause();  
    }

    exit(EXIT_SUCCESS);
}
