
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>



using namespace std;

//uso la ruta del archivo para sacar el nombre del propietario del mismo. Luego esto lo uso para comparar
//ese nombre de propietario con el nombre del usuario logueado para determinar si puede o no realizar las acciones
//solicitadas	

extern void s_inst(char *inst,char *ar[],char op);
extern void ejecutar(char **args);

string owner(char* s_a){	
		string p1="stat -c  --format=%U ", p2= string(s_a);
//		FILE *o = stdout; //guardo antiguo valor de stdout porque con freopen no se puede restaurar
		int o = dup(fileno(stdout));
		string tmp=p1+p2; //armo la instruccion de extraccion
//		cout<<tmp<<endl;
		const char* inst, *metfile;
		inst=tmp.c_str();	//convierto el string a cadena de char				
		int len=tmp.size();
		char* vtmp[len];
		s_inst((char*)inst,vtmp,'e'); //armo el vector de comandos para mandar a ejecutar
		freopen("/tmp/metadatos.txt","w", stdout); //redirijo la salida al archivo
		ejecutar(vtmp);
		dup2(o,fileno(stdout));
		close(o);	
		
		string tmp2="/tmp/metadatos.txt";
		metfile=tmp2.c_str();
	
//		en esta parte extraigo solo el nombre de usuario
		char * nombre= new char[10];
		ifstream line;
		line.open(metfile, ios::in); //abro archivo para lectura 
		line.seekg(9); //me posiciono luego del --format=
		line.getline(nombre,10, '\n'); //leo el nombre (supongo que un nombre de usuario no tendra mas de 10 caracteres...		
		line.close();
		
		string n= string(nombre);
		
		return nombre;
		
}
