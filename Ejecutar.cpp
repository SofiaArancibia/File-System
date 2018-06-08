/** @brief Funcion que ejecuta la instruccion pasada como argumento.
En esta función se utilizan las familias fork y exec. La primera para crear un proceso hijo (la shell es el padre,
el comando a ejecutar "será" el hijo), la segunda para ejecutar la instruccion deseada.
	@author Arancibia, Sofia Casandra
	@date 03/07/2015
*/

#include <wait.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno> 

using namespace std;

void ejecutar(char **args){ //como se separa la instruccion en palabras, se deberá recibir ese vector creado
//el cual se utiliza con la funcion execvp	
	pid_t pid;	
	pid=fork();	
	if (pid == -1) { //error en fork()
		printf("Error en fork()");
		//If it is EXIT_FAILURE, it indicates failure.
		exit(EXIT_FAILURE);
	//Luego de la llamada exitosa a fork(), ambos procesos (hijo y padre) empiezan
	//la ejecucion desde la instruccion que está luego del fork()
	}else if(pid==0){ //proceso hijo
			if (execvp(*args,args) != 0){
				printf("%s\n", (char*) strerror(errno));
				exit(EXIT_FAILURE);
			}else exit(EXIT_SUCCESS);			
	}		
	else //padre -> pid>0
	wait(NULL); //esperar hasta que el hijo termine. Usando 'wait' el padre se entera que el hijo ha terminado
	//y se evita el estado "zombie" (el proceso hijo termino, pero todas su cosas siguen en memoria)
	
}



//Referencias:
//http://www.cplusplus.com/reference/cstdlib/exit/ 
