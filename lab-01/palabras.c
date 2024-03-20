#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
	int c;

	while (c != EOF ){
	
		c = getchar();

		if (c == ' ') { // si "espacio" entonces pasar a sig linea
			printf("%c\n",' ');

		} else {
		
			printf("%c",c);
		}
		
	} 
	// REVISAR!!!!!
    // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
