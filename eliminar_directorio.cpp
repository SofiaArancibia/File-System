#include <vector>
#include "inodo.h"
#include <cstring>
#include <iostream>
using namespace std;

extern vector <inodo> inodos;
extern int buscar_inodo(string nombre);
extern void eliminar_archivo(int indx);

void eliminar_directorio(char* name){
	int nro=-1;
	
	for (unsigned int i=0; i<inodos.size(); i++){
		if((inodos[i].dir) && (inodos[i].nombre==name)){ //busco el directorio entre los inodos			
			nro=inodos[i].nro;		
			break;
		}else{continue;} 		
	}
	
	if(nro==-1){cout<<"Directorio no encontrado"<<endl; return;}
	else{
	
		for (unsigned int i=0; i<inodos.size(); i++){
			if (  !(inodos[i].dir) && (inodos[i].antec==nro) ){ //si es un archivo
				eliminar_archivo(buscar_inodo(inodos[i].nombre));				
			}else if ((inodos[i].dir) &&  (inodos[i].antec==nro) ){ //si es un directorio
				string n=inodos[i].nombre;			
				char * nn = new char[n.length() + 1];
				strcpy(nn,n.c_str());							
				eliminar_directorio(nn);
				inodos[i].ocupado=false;
				inodos[i].dir=false;
				inodos[i].hijos.clear();
				inodos[i].nombre="";				
			}
			inodos[i].nombre="";
		}
		
		return;
	}
	
	
}
