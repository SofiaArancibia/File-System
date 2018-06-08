
#include <string>
#include "inodo.h"
#include <iostream>
#include <cstdio>
#include <cerrno>
#include <cstring>

using namespace std;

extern int buscar_inodo(string nombre);
extern void guardar(FILE* archivo,inodo &i);

void crear_archivo(string destino, char* nombre, inodo &in){	
	char rta; //[y/n]
	FILE* archivo;
	string nom=string(nombre);	
	
	int indx=buscar_inodo(nombre); //busca por nombre, los archivos solo tienen nombre (mi_archivo.txt) 
	
			if(indx!=-1 ){				
				cout<<"El archivo ya existe! Si continua, perdera el contenido del mismo."<<endl;
				cout<<"Desea continuar? [s/n] "<<endl;				
				cin>>rta;
				if(rta == 'n') return;
				else if(rta == 's') {
					archivo=fopen(nom.c_str(), "w"); // abro el archivo para escritura
						if(archivo==NULL) {				
							printf("%s\n", (char*) strerror(errno));						
							return;
						}else{		
							string tmptxt;						
							//Esta dos veces porque el getline me lee el enter ...
							getline(std::cin,tmptxt); //por defecto, '\n' es delimitador		
							getline(std::cin,tmptxt); //por defecto, '\n' es delimitador		
							tmptxt += '\n';
							const char* txt=tmptxt.c_str();	//convierto el string a cadena de char
							fputs( txt, archivo );
							fclose(archivo);
							archivo=fopen(nom.c_str(), "r"); // abro el archivo para lectura
							guardar(archivo,in);
							in.ocupado=true;
							fclose(archivo);
						}
						in.nombre=nom;
						return;						
				}else{
						cout<<"Respuesta incorrecta. "<<endl;
						return;						
					}
			}else{	// si el indice es -1 => no se encontro el archivo, asique creo uno		
				archivo=fopen(nom.c_str(), "w"); // abro el archivo para escritura
				if(archivo==NULL){				
					printf("%s\n", (char*) strerror(errno));
					return;
				}else{		
					string tmptxt;
					//Esta dos veces porque el getline me lee el enter ...
					getline(std::cin,tmptxt); //por defecto ya era '\n'		
					getline(std::cin,tmptxt); //por defecto ya era '\n'		
					tmptxt += '\n';
					const char* txt=tmptxt.c_str();	//convierto el string a cadena de char
					fputs( txt, archivo );
					fclose(archivo);
					archivo=fopen(nom.c_str(), "r"); // abro el archivo para lectura
					guardar(archivo,in);
					in.ocupado=true;
					fclose(archivo);
				}
				in.nombre=nom;				
				//con el mismo nombre en otro directorio				
				return;
			}
} //mover
