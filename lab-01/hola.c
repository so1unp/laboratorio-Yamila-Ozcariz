#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
	// printf("hola mundo\n");
	
	int i;

	for ( i = 1 ; i < argc  ; i = i + 1 ){
		
		if (i < argc - 1) {
		
			printf("%s ",argv[i]);
		
		} else {
			printf("%s\n",argv[i]);
		}
	
	}
	
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
