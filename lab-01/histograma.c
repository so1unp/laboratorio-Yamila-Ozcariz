#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    const int MAX_LETRAS = 25;
    int i; // contador de arr
    int j; 
    int c; // getchar()
    int cletras = 0; // cantidad de letras de una palabra
    int arr[MAX_LETRAS]; // cada posicion de arr[] representa la cant de palabras con n letras

    for (i = 0 ; i < MAX_LETRAS ; ++i){
        arr[i] = 0;
    }

    while (c != EOF) {

        c = getchar();

        if (c != ' ' && c != '\n' && c != EOF) {
            cletras += 1;
        } else {
            if (cletras > 0) {
                arr[cletras - 1] += 1;
                cletras = 0;
            }
        }

    }
  
    for (i = 0 ; i < MAX_LETRAS ; ++i) {
        if (arr[i] != 0){

            printf("%d.",i+1);
            for (j = 0 ; j < arr[i] ; ++j ){
                printf("%c", '*');
            }
            printf("%c\n", ' ');

        } 

    }
  
  
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
