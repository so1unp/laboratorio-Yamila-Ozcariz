#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS 10 // cantidad max de procesos a crear
#define MV_SIZE     64  // tamano de memoria virtual
#define MF_SIZE     32  // tamano de memoria fisica
#define AS_SIZE     64  // tamano de almacenamiento secundario
#define PAG_SIZE    4   // tamano de pagina

int c; // getchar()

//char MV[MV_SIZE / PAG_SIZE];    // arreglo de memoria virtual
//char MF[MF_SIZE / PAG_SIZE];    // arreglo de memoria fisica
//char AS[AS_SIZE / PAG_SIZE];    // arreglo almacenamiento secundario

// hacer structs para tabla de paginas y memoria fisica
struct process {
    int pid;
    int paginas[MV_SIZE / PAG_SIZE];
    char destino;
}; typedef struct process process_t;

struct memory {
    int pid;
    int pagina;
}; typedef struct memory memory_t;


int main(int argc, char* argv[])
{
    if (argc > 2) {
        perror("cantidad de argumentos invalida.");
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        perror("comando erroneo '-'.");
        exit(EXIT_FAILURE);
    }

    if(argv[1][1] != 'f' || argv[1][1] == 'l'){ // verificar algoritmo de reemplazo de pag
        perror("algoritmo de reemplazo erroneo.");
        exit(EXIT_FAILURE);
    }


    process_t tabla_p[MAX_PROCESS];                 // tabla de procesos
    memory_t memoria_f[MF_SIZE / PAG_SIZE];         // memoria fisica
    memory_t almacenamiento_s[AS_SIZE / PAG_SIZE];  // almacenamiento secundario
    memory_t insert;
    int i,j,c;
    int lleno_mf=0, lleno_as=0; // indica si estan llenas las memorias 

    for (i=0 ; i < MAX_PROCESS ; i++ ) {    // inicializo tabla de paginas
        tabla_p[i].pid = 0;
        for (j=0 ; j < MV_SIZE / PAG_SIZE ; j++ ) {
            tabla_p[i].paginas[j] = 0 ;
        }
    }
    for (i=0 ; i < MF_SIZE / PAG_SIZE ; i++) {
        memoria_f[i].pid = 0;
        memoria_f[i].pagina = 0;
        
    }
    for (i=0 ; i < AS_SIZE / PAG_SIZE ; i++) {
        almacenamiento_s[i].pid = 0;
        almacenamiento_s[i].pagina = 0;
    }
    
    switch (argv[1][1]) {
        case 'f':   // algoritmo de reemplazo de páginas First In First Out

            while (c = getchar() != EOF) {
                
                //scanf("%d\n%d",&insert.pid,&insert.pagina);
                //pid = getchar();
                if(insert.pid < 0 && insert.pid > MAX_PROCESS) {      // valido identificador de proceso
                    perror("identificador de proceso no valido.");
                    exit(EXIT_FAILURE);
                }

                //pag = getchar();
                if(insert.pagina < 0 && insert.pagina > MV_SIZE / PAG_SIZE) {  // valido identificador de pagina
                    perror("identificador de pagina no valido.");
                    exit(EXIT_FAILURE);
                }



                if (lleno_mf == MF_SIZE / PAG_SIZE && lleno_as == AS_SIZE / PAG_SIZE) { // memoria fisica y almacenamiento secundario lleno
                    printf("Memoria fisica y Almacenamiento Secundario llenos, no se puede insertar mas paginas.");
                    break;

                } else {
                    if (lleno_mf != MF_SIZE / PAG_SIZE) {    // verifica si la memoria fisica esta llena 
                        for (i = 0 ; i < MF_SIZE / PAG_SIZE ; i++) {
                            if (memoria_f[i].pid == 0) {    // recorro memoria fisica hasta encontrar un espacio libre
                                memoria_f[i] = insert;
                                lleno_mf++;
                                tabla_p[insert.pid - 1].destino = 'f';
                                break;
                            } 
                        }
                    } else if (lleno_as != AS_SIZE / PAG_SIZE) { // verifica si el almacenamiento secundario esta lleno  
                        for (i = 0 ; i < AS_SIZE / PAG_SIZE ; i++) {
                            if (almacenamiento_s[i].pid == 0) {    // recorro almacenamiento secundario hasta encontrar un espacio libre
                                almacenamiento_s[i] = insert;
                                lleno_as++;
                                tabla_p[insert.pid - 1].destino = 's';
                                break;
                            }
                             
                        }
                    }
                    tabla_p[insert.pid - 1].pid =insert.pid;
                    tabla_p[insert.pid - 1].paginas[insert.pagina - 1] = i+1;
                }
            }

            break;
        case 'l':   // algoritmo de reemplazo de páginas Last Recently Used
            //while () {

            //}
            break;
      
    }

    printf("Informacion de tabla de paginas, memoria fisica y almacenamiento secundario:\n");

    printf("Tabla de paginas:\n");
    for (i = 0; i < MAX_PROCESS ; i++) { // imprimo tabla de procesos
        if(tabla_p[i].pid != 0) {   // si existe el proceso en la tabla
            printf("Proceso %d: ", tabla_p[i].pid);

            for (j = 0 ; j < MV_SIZE / PAG_SIZE ; j++) {
                if(tabla_p[i].paginas[j] != 0) {    // si existe la pagina almacenada
                    printf("%c.%d ", tabla_p[i].destino, tabla_p[i].paginas[j]);
                } else {
                    printf("- ");
                }
            }
            printf("\n");
        } 
    }
    
    printf("Memoria Fisica:\n");
    for (i = 0; i < MF_SIZE / PAG_SIZE ; i++) { // imprimo memoria fisica
        if (memoria_f[i].pid != 0) {    // si existe el proceso en memoria
            printf("%d.%d ", memoria_f[i].pid, memoria_f[i].pagina);
        } else {
            printf("- ");
        }
    }
    printf("\n");
    
    printf("Almacenamiento Secundario:\n");
    for (i = 0; i < AS_SIZE / PAG_SIZE ; i++) { // imprimo memoria secundaria
        if(almacenamiento_s[i].pid != 0) { // si existe el proceso en almacenamiento
            printf("%d.%d ", almacenamiento_s[i].pid, almacenamiento_s[i].pagina);
        } else {
            printf("- ");
        }
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
