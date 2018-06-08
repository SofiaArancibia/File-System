// Con esta funcion busco entre los inodos el que corresponde al archivo que quiero usar. 
// Devuelve el indice del inodo

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "inodo.h"

using namespace std;

extern vector <inodo> inodos;

int buscar_inodo(string nombre){	
	int indx=-1;
	for (unsigned int i=0; i< inodos.size(); i++){
		inodo in= inodos[i];
		if (in.nombre == nombre){
			indx=i;
			return indx;
		}
	}
	return indx;
}


