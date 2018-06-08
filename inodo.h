#ifndef INODO_H
#define INODO_H
#include <string>
#include <vector>

using namespace std;

extern unsigned int c_inodos;

class inodo{
public:
	//por defecto desocupado. Esto lo modifico en la funcion guardar
	unsigned int nro; //identificador del inodo
	bool ocupado; 
	bool raiz; 
	bool dir; 
	string nombre;	
	string usuario;
	vector<int>enlaces; // me guardo 4 indices de bloque, y 2 indices de enlace (indirecto simple y doble)	
	int antec; //indica a que directorio pertenece
	vector<int>hijos; //para directorios
	
	inodo(){
		nro=c_inodos++;	
		ocupado=false;
		raiz=false;
		dir=false;
	}	
};






#endif
