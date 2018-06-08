#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "inodo.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include "bloque.h"
#include <stdlib.h> 

using namespace std;

// Cada bloque es de un 1kb
// Un bloque de 1kb es un vector de 1024 unsigned char

vector < bloque > datos(64); //vector de datos
vector< vector<int> > enlaces(2000); //para enlaces indirectos simples y dobles
vector <inodo> inodos(1000); //vector de inodos
bool infob=false;
bool admin=false;
string psw="admin";
int sobra;
bool hay_sobra;
unsigned int c_inodos=-1;
	
extern void guardar(FILE* archivo,inodo &i);
extern void s_inst(char *inst,char *ar[],char op);
extern void help();
extern int buscar_inodo(string nombre);
extern int buscar_directorio(string arg);	
extern void ejecutar(char **args);
extern int buscar_sig(string que);
extern string owner(char* s_a);
extern void eliminar_archivo(int indx);
extern void crear_archivo(string destino, char* nombre, inodo &in);
extern void eliminar_directorio(char* name);

inodo crear_fs(){
	inodo RAIZ;
	RAIZ.nro=0;
	RAIZ.raiz=true;
	RAIZ.dir=true;
	RAIZ.antec=-1; //solo para raiz;	
	RAIZ.nombre="/home/sofia/Escritorio/RAIZ";		
	inodos.push_back(RAIZ);
//	c_inodos++;		
	return RAIZ;
}

int main(){		
	printf("___________________________________\n");
	printf("so2016fileSystem\n");
	printf("Autor: ARANCIBIA, Sofia Casandra\n");
	printf("Version: 1.0\n");
	printf("Año: 2016\n");
	printf("___________________________________\n");
	
	printf("\nPresione 'h' para consultar la ayuda\n\n");

	
	
	
	inodo RAIZ=crear_fs();
	
	while(true){		
		
		//_________________________________________
		if(!admin){
			printf("%% "); //prompt			
		}else{
			printf("Admin%% ");
		}
		//_________________________________________
		
		// FORMATO: [comando + ruta_del_archivo]		
		string com; 
		FILE* archivo; //puntero a tipo de dato FILE				
		const char* ruta;			
		string us_login=getlogin(); //obtengo el nombre de usuario logueado	para determinar luego que acciones puede
		//realizar sobre los archivos.
		
		//leo hasta el enter
		getline(std::cin,com,'\n'); //por defecto ya era '\n'			
		
		
		
		if(com.length()==0) getline(std::cin,com,'\n'); //si presiono enter y no hay ningun comando, va de nuevo...	
		
		if (com=="exit"){
			if(admin){
				admin=false;
				continue;
			}
			exit(EXIT_SUCCESS);
			return 0;			
		}
		
		
		if(com.length()==1){ 
			if(com=="h"){
				help();
				continue;
			}else if (com=="i"){
				infob=!infob;
				continue;
			}else if(com=="a"){
				if (admin==true){
					continue;
				}else{
					string tmp_psw;
					cout<<"Contraseña: ";
					getline(std::cin,tmp_psw,'\n');
					if(tmp_psw==psw){
						admin=true;
						continue;
					}else{
						cout<<"Contraseña incorrecta"<<endl;
						continue;
					}
				}
			}else{
				cout<<"Comando desconocido "<<endl;
				continue;
			}
		}
		
		ruta=com.c_str();	//convierto el string a cadena de char
		
		//arreglo resultado de s_inst
		//el tamaño maximo será lo que ocupa la instruccion completa. Así me aseguro de
		//no guardar espacio de menos.				
		int len=com.size();
		char* s_a[len];			
		s_inst((char*)ruta,s_a,'e'); //creo arreglo de argumentos
		//en este caso lo correcto es tener solo dos elementos [c,n,d...] + la ruta del archivo ( s_a[1] )
				
		//************PROPIETARIO DEL ARCHIVO********************************
		string prop;
		if (*s_a[0]=='c' || *s_a[0]=='d' ){ prop=owner(s_a[1]); } 
		//el propietario del archivo puede realizar cualquier accion sobre el, al igual que el Adminisitrador o root
		//un no propietario solo puede verlo
		//*******************************************************************
				
		//control de comando ingresado en formato correcto
		if (s_a[2]){ //si tengo un tercer elemento en el vector, entonces hay algo mal
			cout<<"Comando ingresado incorrecto. Intentelo nuevamente. "<<endl;
			continue;
		}
		
		//Las acciones siguientes se aplican sobre el archivo especificado en la RUTA. Si solo se provee el nombre del archivo, 
		//este se busca en el directorio actual y a partir de alli se crea/borra/..
				
		
		
		
		
		switch(*s_a[0]){ 
		case 'c': //cargar archivo (existente) en el sistema	
				  //Por defecto los archivos se cargan en el directorio raiz
		{		  //Se pasa ruta completa del archivo
			if (!admin){
				cout<<"No tiene permiso de realizar esta accion"<<endl;
				continue;
			}else{
				//(*)
				inodo in;
				archivo=fopen(s_a[1], "r"); // abro el archivo para lectura
				if(archivo==NULL) {				
					printf("%s\n", (char*) strerror(errno));
					continue;
				}else{	
					cout<<"Inodo: "<<in.nro<<endl;
					guardar(archivo,in); //guardo el archivo en el bloque de datos				  
					in.ocupado=true;
					in.antec=RAIZ.nro;
				}
				
				//_________________________________________________
				//Esto es para que fisicamente se meta el archivo en la carpeta raiz
				string f=string(s_a[1]),ff;
//				ff= "cp -a " + f + " /home/sofia/Escritorio/RAIZ";
//				system(ff.c_str());
				//_________________________________________________
				
				//SEPARo EL NOMBRE DEL ARCHIVO SOLO!
				
				
				//___________________
				
				
				const char * dest=f.c_str();
				char* d[f.size()];			
				s_inst((char*)dest,d,'b');
				
				//contar elementos en el vector
				int cant=0;
				while(d[cant]!='\0'){				
//					cout<<d[cant];
					cant++;					
				}
				//_____________________________
				
				int cont=0;
				string ex="/";
				while(cont!=cant-1){
					ex+=d[cont];				
					cont++;
					if(cont!=cant-1) ex+="/";
				}
				//____________________________________
				
				in.nombre=d[cant-1]; 
//				cout<<d[cant-1];
				//con el mismo nombre en otro directorio	
				in.usuario=prop;
								
				if (inodos.empty()){
					inodos.push_back(in);					
				}else{
					int indx=buscar_sig("inodo");
					inodos[indx]=in;					
				}			
				RAIZ.hijos.push_back(in.nro);
				fclose(archivo);
				break;
			
			}	
		}
			
		case 'v': //aca no tengo que abrir ningun archivo, sino recorrer los bloques de datos que corresponden
		{
		//me pasan la ruta completa => CORREGIR ! PARA QUE BUSQUE EL DIRECTORIO QUE LO CONTIENE	
			int indx;
			inodo obj; //obj de objetivo (?
			vector<int> bl;
			string n= string(s_a[1]);
			indx=buscar_inodo(n);
			if(indx==-1 || inodos[indx].ocupado == false){
				cout<<"Archivo no encontrado! "<<endl;
				break;
			}else{
				obj=inodos[indx];
				bl=obj.enlaces;		
				int t=bl.size();
				
				if (t<=4){ //es que solo use enlaces directos
					for (int i=0; i<t; i++){
						int indice=bl[i];
						vector < unsigned char > tmp = datos[indice].b;
						for(unsigned int j=0; j<tmp.size(); j++){
							cout<<tmp[j];
						}
					}
				}else if (t==5) {//directos e indirecto simple
					for (unsigned int i=0; i<4; i++){ //parte directos
						int indice=bl[i];
						vector < unsigned char > tmp = datos[indice].b;
						for(unsigned int j=0; j<tmp.size(); j++){
							cout<<tmp[j];
						}
					}
					//parte indirectos simples
					int tmpe=bl[4]; //saco el indice del eis
					vector<int> tmp_eis=enlaces[tmpe];
					for (unsigned int i=0; i<4; i++){ 
						int indice=tmp_eis[i];
						vector < unsigned char > tmp = datos[indice].b;
						for(unsigned int j=0; j<tmp.size(); j++){
							cout<<tmp[j];
						}
					}					
					
				}else if (t==6){//directos , indirecto simple, indirecto doble
					
					for (unsigned int i=0; i<4; i++){ //parte directos
						int indice=bl[i];
						vector < unsigned char > tmp = datos[indice].b;
						for(unsigned int j=0; j<tmp.size(); j++){
							cout<<tmp[j];
						}
					}
					//parte indirecto simple
					int tmpe=bl[4]; //saco el indice del eis
					vector<int> tmp_eis=enlaces[tmpe];
					for (unsigned int i=0; i<4; i++){ 
						int indice=tmp_eis[i];
						vector < unsigned char > tmp = datos[indice].b;
						for(unsigned int j=0; j<tmp.size(); j++){
							cout<<tmp[j];
						}
					}
					
					//parte indirecto doble
					
					tmpe=bl[5]; //saco el indice del eid					
					vector<int> tmp_eid=enlaces[tmpe];		//vector de enlaces id			
					for (unsigned int i=0; i<tmp_eid.size(); i++){
						vector<int> tmp_eis=enlaces[tmp_eid[i]]; //vector de enlaces d
						for (unsigned int j=0; j<tmp_eis.size(); j++){ 
							int indice=tmp_eis[j]; //indice de bloque de datos
									vector < unsigned char > tmp = datos[indice].b;									
									for(unsigned int l=0; l<tmp.size(); l++){
										cout<<tmp[l];
									}
									
								}							
						}
								
					}
				cout<<endl;
							
				
				break;					
			}			
		}
		case 'n':  //para archivos solamente
		{			
			int nro; //del nodo que voy a buscar 	
			inodo in;			
			string destino; //nombre de la carpeta padre			
			int indice_padre;
			
			cout<<"Destino [/dir1/dir1.1/dir1.1.3...]: ";
			cin>>destino;	
			nro=-1;
			
			for (unsigned int i=0; i<inodos.size(); i++){ //busco antecesor				
				if((inodos[i].dir) && (inodos[i].nombre==destino)){ //los inodos correspondientes a directorios tienen en el campo 'nombre' la ruta completa del mismo desde la raiz
					nro=inodos[i].nro;
					indice_padre=i;
				}
			}
			
			if (nro==-1) {cout<<"Directorio no encontrado "<<endl; continue;}
			else{
				in.antec=nro; 
				in.nombre=string(s_a[1]);
				
				crear_archivo(destino,s_a[1], in); //destino, nombre
				cout<<"Inodo: "<<in.nro<<endl;
				in.usuario=us_login;				
				if (inodos.empty()){
					inodos.push_back(in);
				}else{
					int indx=buscar_sig("inodo");
					inodos[indx]=in;
				}
			}				
			inodos[indice_padre].hijos.push_back(in.nro); //en los hijos del directorio me guardo numero de inodo del archivo que cree recien
			//para listar puedo usar la lista de hijos o el numero de antecesor.. 
			
			break;
		}
		
		case 'd': //para archivos
		{
			cout<<(us_login != prop)<<" "<<(admin==false)<<endl;
			if((us_login != prop) and admin==false){
				cout<<"No tiene permiso para realizar esta accion"<<endl; //solo el propietario del archivo puede eliminarlo
				break;
			}else{
				string n= string(s_a[1]);
				//ME PASAN LA RUTA COMPLETA Y TENGO QUE SEPARAR EL NOMOBRE DEL ARCHIVO DE LA RUTA (RUTA DEL DIRECTORIO ANTECESOR)
				//ASI PUEDO BUSCAR EL ANTECESOR Y RECIEN AHI ELIMINAR EL ARCHIVO. ESTO ES PORQUE PUEDE HABER DOS ARCHIVOS CON EL 
				//MISMO NOMBRE EN CARPETAS DIFERENTES
				
				
				//___________________
				
				//busco el directorio antecesor (la carpeta que contiene al archivo)
				
				const char * dest=n.c_str();
				char* d[n.size()];			
				s_inst((char*)dest,d,'b');
				
				//contar elementos en el vector
				int cant=0;
				while(d[cant]!='\0'){				
					cant++;
				}
				//_____________________________
				
				int cont=0,nro;
				string ex="/";
				while(cont!=cant-1){
					ex+=d[cont];				
					cont++;
					if(cont!=cant-1) ex+="/";
				}
				
				nro=-1;
				
				for (unsigned int i=0; i<inodos.size(); i++){ //busco antecesor					
					if((inodos[i].dir) && (inodos[i].nombre==ex)){ //los inodos correspondientes a directorios tienen en el campo 'nombre' la ruta completa del mismo desde la raiz
						nro=inodos[i].nro;
					}
				}
				if(nro==-1){
					nro=RAIZ.nro;
				}
				
				string name=d[cant-1];

				//___________________
				
				//nro tiene el numero de inodo del padre del archivo. Entonces ahora busco el archivo con ese nombre y ese padre
				int indx=-1;
				for (int i=0; i<inodos.size(); i++){					
					if( !(inodos[i].dir) && inodos[i].antec==nro && inodos[i].nombre==name){
						indx=i;
					}
				}
				
				if (indx==-1){ //no encontre el archivo en el sistema
					cout<<"No se encontro el archivo o no existe "<<endl;
					continue;
				}else{				
					cout<<"El archivo se eliminara del sistema."<<endl;
					cout<<"Desea continuar? [s/n]"<<endl;			
					char rta;
					cin>>rta;			
					if (rta=='s'){		
						eliminar_archivo(indx);
						continue;
					}else if(rta=='n'){					
						continue;
					}else{
						cout<<"Respuesta incorrecta"<<endl;
						continue;
					}
				}
				break;	
			}
		}		
		case 'm': //crear directorio
		//en este caso en s_a[1] tengo que tener la ruta completa del directorio <- eso es su nombre	
		{
			int nro;
			string str= string(s_a[1]), inst;
			inodo indir;			
			indir.dir=true;			
			indir.nombre= str;
			indir.usuario=us_login;
			indir.ocupado=true;
						
			//busco el directorio antecesor 
			
			const char * dest=str.c_str();
			char* d[str.size()];			
			s_inst((char*)dest,d,'b');
			
			//contar elementos en el vector
			int cant=0;
			while(d[cant]!='\0'){				
				cant++;
			}
			//_____________________________
			
			int cont=0;
			string ex="/";
			while(cont!=cant-1){
				ex+=d[cont];				
				cont++;
				if(cont!=cant-1) ex+="/";
			}			

			nro=-1;
			for (unsigned int i=0; i<inodos.size(); i++){ //busco antecesor				
				if((inodos[i].dir) && (inodos[i].nombre==ex)){ //los inodos correspondientes a directorios tienen en el campo 'nombre' la ruta completa del mismo desde la raiz
					nro=inodos[i].nro;
					break;
				}
			}
			
			indir.antec=nro;
			
			if (inodos.empty()){
				inodos.push_back(indir);
			}else{
				int indx=buscar_sig("inodo");
				inodos[indx]=indir;
			}			
			
			cout<<"Inodo: "<<indir.nro<<endl;
			
			break;
		}
		case 'r': //eliminar directorio. Es recursivo.. Asique borra todo lo que tiene adentro 
			// se indica en s_a[1] la ruta completa del directorio a eliminar
		{		
			
			eliminar_directorio(s_a[1]);			
			break;
		}
		
		case 'l':
		{
			
			//me pasan la ruta de la carpeta que quiere inspeccionar
			int n=-1;
			string nombre=string(s_a[1]);
			for(int i=0; i<inodos.size(); i++){ //busco el numero de inodo (identificador) del directorio a inspeccionar
				if(	(inodos[i].dir) && (inodos[i].nombre==nombre) ){
					n=inodos[i].nro;
					break;
				}else{continue;}
			}
			
			if (n==-1) { cout<<"Directorio no encontrado"<<endl; continue; }
			
			//una vez que tengo el numero del directorio, busco los hijos
			
			string p1d="\033[1;31m";  //directorios en rojo
			string p2d="\033[0m\n";
			string salidaD;
			
			string reset="\033[0;31m  \033[0m\n";
			
			string p1a="\033[1;34m"; //archivos en azul
			string p2a="\033[0m\n";
			string salidaA;
			
			
			
			for(int i=0; i<inodos.size(); i++){
				if(inodos[i].antec==n){ //si el inodo tiene a este directorio como padre...
					if(inodos[i].dir){ //si es directorio muestro el nombre de un color
						salidaD=	p1d+	inodos[i].nombre + p1d;
						cout<<salidaD;
						
					}else if (!(inodos[i].dir)){ //si es archivo de otro
						salidaA=	p1a+	inodos[i].nombre + p1a;
						cout<<salidaA;
						
					}
					
					cout<<reset;
				}
			}
			break;
		}
		
		default:
			{cout<<"Comando desconocido. Ingrese 'h' para recibir ayuda. "<<endl;
			break;}			
		}//switch		
	}//while	
	
	
	
}//main



// Referencias C/C++
/// FILE * fopen( const char * filename, const char * mode ) ;
//Opens a file indicated by filename and returns a file stream associated 
// with that file. mode is used to determine the file access mode. 
/// int fseek( std::FILE * stream, long offset, int origin ) ;
