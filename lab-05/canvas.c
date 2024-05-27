#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>

#define HEIGHT  25  // Altura en caracteres de la pizarra
#define WIDTH   25  // Ancho en caracteres de la pizarra

struct canvas {
    char canvas[HEIGHT*WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];
    int shm, i, j;
    char *mmappt;
    
    canvas_t sh_canvas;

    for (i = 0; i < HEIGHT*WIDTH ; i++) {
        sh_canvas.canvas[i] = '*';
    }

    switch(option) {
        case 'w':
            printf("Escribe %s en el canvas %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));
            int x,y;
            x = atoi(argv[4]);
            y = atoi(argv[5]);

            if (x < 0 || x > WIDTH-1 || y < 0 || y > HEIGHT-1) {
                perror("Posiciones de canvas invalidas.");
                exit(EXIT_FAILURE);
            }

            shm = shm_open(argv[2], O_WRONLY,0622);
            if (shm == -1) {
                perror("shm_open");
                exit(EXIT_FAILURE);
            }

            mmappt = mmap(sh_canvas.canvas, sizeof(sh_canvas.canvas), PROT_WRITE, MAP_SHARED, shm, x + y*HEIGHT);
            if (mmappt == MAP_FAILED) {
                perror("mmap");
                exit(EXIT_FAILURE);
            }

            memcpy(mmappt, argv[3], strlen(argv[3]));

            close(shm);
            break;
        case 'p':
            printf("Imprime canvas.\n");

            shm = shm_open(argv[2], O_RDONLY,0622);
            if (shm == -1) {
                perror("shm_open");
                exit(EXIT_FAILURE);
            }

            mmappt = mmap(sh_canvas.canvas, sizeof(sh_canvas.canvas), PROT_READ, MAP_SHARED, shm, 0);
            if (mmappt == MAP_FAILED) {
                perror("mmap");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < HEIGHT ; i++) {
                for(j = 0 ; j < WIDTH ; j++) {
                    printf("%c ", sh_canvas.canvas[i +j]);
                    if (j == WIDTH-1) {
                        printf("\n");
                    }
                }
            }

            close(shm);
            break;
        case 'c':
            printf("Crea canvas.\n");
            int size;
            
            shm = shm_open(argv[2], O_CREAT,0622);
            if (shm == -1) {
                perror("shm_open");
                exit(EXIT_FAILURE);
            }

            size = ftruncate(shm, sizeof(sh_canvas.canvas));
            if (size == -1) {
                perror("ftruncate");
                exit(EXIT_FAILURE);
            }

            close(shm);
            break;
        case 'd':
            printf("Borra canvas.\n");

            shm = shm_unlink(argv[2]);
            if (shm == -1) {
                perror("shm_unlink");
                exit(EXIT_FAILURE);
            }

            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido\n");
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
