// Funcion que busca el siguiente "que" vacio

#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "inodo.h"
#include "bloque.h"

using namespace std;


extern vector <inodo> inodos;
extern vector < bloque > datos; //(64);  //vector de datos
extern vector< vector<int> > enlaces; //(2000); //para enlaces indirectos simples y dobles


int buscar_sig(string que){
	if (que=="bloque"){
		int indx=-1;
			for (unsigned int i=0; i< datos.size(); i++){		
				if(!datos[i].ocupado){
					indx=i;
					return indx;
				}
			}
			return indx;		
	}else if (que=="inodo"){
		int indx=-1;
		for (unsigned int i=0; i< inodos.size(); i++){		
			inodo in=inodos[i];
			if (in.ocupado==false){
				indx = i;
				return indx;
			}
		}
		return indx;
	}else if(que=="enlace"){
		int indx=-1;
		for (unsigned int i=0; i<enlaces.size(); i++){
			if (enlaces[i].empty()){
				indx=i;
				return indx;
			}
		}
		return indx;
	} else if(que=="enlace++"){
		int indx=-1;
		for (unsigned int i=0; i<enlaces.size()-1; i++){
			if (enlaces[i+2].empty()){
				indx=i+2;
				return indx;
			}
		}
		return indx;
		
	}

}
