#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int fd,uid,iid,bsize,bdisk;
char ftype[30];
time_t mod,acc;
struct stat stat_buf;

int main(int argc, char *argv[])
{

    if (argc > 2) {
        perror("Cantidad de argumentos invalida.");
        exit(EXIT_FAILURE);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &stat_buf) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    uid = stat_buf.st_uid;
    iid = stat_buf.st_ino;
    bsize = stat_buf.st_size;
    bdisk = stat_buf.st_blocks;
    mod = stat_buf.st_mtim.tv_sec;
    acc = stat_buf.st_atim.tv_sec;
    
    switch(stat_buf.st_mode & S_IFMT) {
        case S_IFREG:
            strcpy( ftype, "archivo regular");
            break;
        case S_IFDIR:
            strcpy( ftype, "directorio");
            break;
    }

    printf("ID usuario: %d\nTipo de archivo: %s\nNro i-nodo: %d\nTamaño en bytes: %d\nBloques en disco: %d\nÚltima modificación: %s\nÚltimo Acceso: %s\n", 
    uid, ftype, iid, bsize, bdisk,ctime(&mod),ctime(&acc));

    close(fd);

    exit(EXIT_SUCCESS);
}
