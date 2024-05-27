#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>          
#include <sys/stat.h>        
#include <mqueue.h>
#include <string.h>

#define USERNAME_MAXSIZE    15  // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE            100 // Máximo tamaño del texto del mensaje.
#define MSG_MAXSIZE         10  // Maxima cantidad de mensajes en la cola

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
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

    mqd_t mq;

    struct mq_attr mqattr;
    mqattr.mq_flags = O_NONBLOCK;     // ignorar
    mqattr.mq_maxmsg = MSG_MAXSIZE;
    mqattr.mq_msgsize = USERNAME_MAXSIZE + TXT_SIZE + 3;
    mqattr.mq_curmsgs = 0;   // ignorar

    char msg[USERNAME_MAXSIZE + TXT_SIZE + 3];
    unsigned int prio=0;
    struct mq_attr mqattr_receive;
    int receive;

    switch(option) {
        case 's':
            printf("Enviar %s a la cola %s\n", argv[3], argv[2]);
            msg_t mensaje;
            char usender[USERNAME_MAXSIZE];
            int send;

            if (argc == 5) {
                prio = (unsigned int) atoi(argv[4]);
            }

            mq = mq_open(argv[2], O_WRONLY);
            if (mq == -1) {     // verifica si existe la cola de mensajes
                perror("mq_open");
                exit(EXIT_FAILURE);
            }

            getlogin_r(usender, USERNAME_MAXSIZE); // obtengo mi nombre de usuario
            strcat(mensaje.sender, usender);
            strcat(mensaje.text, argv[3]);

            //msg = mensaje.sender + " : " + mensaje.text;

            strcat(msg, mensaje.sender);
            strcat(msg, " : ");
            strcat(msg, mensaje.text);

            send = mq_send(mq, msg, strlen(msg) + 1, prio);
            if (send == -1) {     // verifica si se envia el mensaje
                perror("mq_send");
                exit(EXIT_FAILURE);
            }

            mq_close(mq);

            break;
        case 'r':
            printf("Recibe el primer mensaje en %s\n", argv[2]);
            
            mq = mq_open(argv[2], O_RDONLY);
            if (mq == -1) {     // verifica si existe la cola de mensajes
                perror("mq_open");
                exit(EXIT_FAILURE);
            } 
            mq_getattr(mq, &mqattr_receive); 
            mq_setattr(mq, &mqattr, &mqattr_receive);

            receive = mq_receive(mq, msg, mqattr.mq_msgsize, NULL);
            if (receive == -1) { // verifica si se pudo recibir un mensaje
                perror("mq_receive");
                exit(EXIT_FAILURE);
            }
            printf("%s \n", msg);   

            mq_close(mq);
            break;
        case 'a':
            printf("Imprimir todos los mensajes en %s\n", argv[2]);
            
            mq = mq_open(argv[2], O_RDONLY);
            if (mq == -1) {     // verifica si existe la cola de mensajes
                perror("mq_open");
                exit(EXIT_FAILURE);
            }
            mq_getattr(mq, &mqattr_receive);
            while (mq_receive(mq, msg, mqattr_receive.mq_msgsize, NULL) != -1) {
                printf("%s \n", msg);
            }

            mq_close(mq);
            break;
        case 'l':
            printf("Escucha indefinidamente por mensajes\n");

            mq = mq_open(argv[2], O_RDONLY);
            if (mq == -1) {     // verifica si existe la cola de mensajes
                perror("mq_open");
                exit(EXIT_FAILURE);
            }

            mq_getattr(mq, &mqattr_receive);
            receive = mq_receive(mq, msg, mqattr_receive.mq_msgsize, NULL);
            if (receive == -1) { // verifica si se pudo recibir un mensaje
                perror("mq_receive");
                exit(EXIT_FAILURE);
            }
            printf("%s \n", msg);

            mq_close(mq);
            break;
        case 'c':
            printf("Crea la cola de mensajes %s\n", argv[2]);

            umask(0000);
            mq = mq_open(argv[2], O_CREAT, 0622, &mqattr);
            if (mq == -1) {     // verifica si se crea la cola de mensajes
                perror("mq_open");
                exit(EXIT_FAILURE);
            }

            mq_close(mq);

            break;
        case 'd':
            printf("Borra la cola de mensajes %s\n", argv[2]);
            int unlink;

            unlink = mq_unlink(argv[2]);
            if (unlink == -1) {
                perror("mq_unlink");
                exit(EXIT_FAILURE);
            }
            
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
