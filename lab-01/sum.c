#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.

	int i;
	int s = 0; // suma de los valores dados

	for(i = 0 ; i < argc ; i = i + 1) {
	
		s = s + atoi(argv[i]);
	
	}

	printf("El resultado es: %d\n",s);



    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
