#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "inodo.h"
#include <algorithm>
#include "bloque.h"

using namespace std;

extern vector < bloque > datos; //(64); //vector de datos
extern vector< vector<int> > enlaces; //(2000); //para enlaces indirectos simples y dobles
extern int buscar_sig(string que);
extern bool infob;
extern int sobra;
extern bool hay_sobra;
extern vector <inodo> inodos;

void eliminar_archivo(int indx){
	inodo obj; //obj de objetivo (?
	vector<int> bl;
	obj=inodos[indx]; //recupero el inodo
	bl=obj.enlaces;	//recupero el vector de enlaces del inodo
	int t=bl.size();
	
	if (t<=4){						
		for (unsigned int i=0; i<t; i++){
			int indice= bl[i];
			datos[indice].clear();
			datos[indice].ocupado=false;							
		}
		obj.enlaces.clear();
	}else if (t==5){						
		//parte directos
		for (unsigned int i=0; i<4; i++){
			int indice= bl[i];
			datos[indice].clear();
			datos[indice].ocupado=false;							
		}						
		//parte indirectos simples
		int tmpe=bl[4]; //saco el indice del eis
		vector<int> tmp_eis=enlaces[tmpe];
		for (unsigned int i=0; i<tmp_eis.size(); i++){ 
			int ii=tmp_eis[i];
			datos[ii].clear();
			datos[ii].ocupado=false;
		}
		enlaces[tmpe].clear();
		obj.enlaces.clear();
		
	}else if (t==6){
		
		//parte directos
		for (unsigned int i=0; i<4; i++){
			int indice= bl[i];
			datos[indice].clear();
			datos[indice].ocupado=false;							
		}						
		//parte indirectos simples
		int tmpe=bl[4]; //saco el indice del eis
		vector<int> tmp_eis=enlaces[tmpe];
		for (unsigned int i=0; i<tmp_eis.size(); i++){ 
			int ii=tmp_eis[i];
			datos[ii].clear();
			datos[ii].ocupado=false;
		}
		enlaces[tmpe].clear();	
		
		tmpe=bl[5]; //saco el indice del eid					
		vector<int> tmp_eid=enlaces[tmpe];					
		for (unsigned int i=0; i<tmp_eid.size(); i++){
			vector<int> tmp_eis=enlaces[tmp_eid[i]];
			for (unsigned int j=0; j<tmp_eis.size(); j++){ 
				int indice=tmp_eis[j];
				datos[indice].clear();
				datos[indice].ocupado=false;								
			}
			enlaces[tmp_eid[i]].clear();							
		}
		enlaces[tmpe].clear();					
	}
	
	inodos[indx].enlaces.clear();
	inodos[indx].ocupado=false;	
	inodos[indx].nombre="";	
	
}
