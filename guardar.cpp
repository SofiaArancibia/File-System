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


void limpiar(vector<unsigned char> &v){
	int l=v.size();
	for(int i=0;i<l;i++){
		v[i]='\0'; //caracter nulo
	}
}

void guardar(FILE* archivo,inodo &in){
	 int tamanio; //variable donde voy a guardar el tamanio del archivo (en bytes)		
	 vector <unsigned char> aux1(1024); //buffer auxiliar que uso en fread	 	 	 
	 size_t tam_bl=1024; //tamaño del bloque que voy a leer
	 fseek(archivo,0, SEEK_END); // me ubico en el final del archivo.
	 tamanio=ftell(archivo); // obtengo su tamanio en BYTES.			
	 int cant_bloques= tamanio/1024; //calculo cuantos bloques completos necesito
	 fseek(archivo,0, SEEK_SET); //vuelvo al inicio del archivo
	 /*int */ sobra=tamanio%1024; //resto 
	 /*bool*/ hay_sobra=false;

	 
	 int tot_bloques=cant_bloques;
	 if(sobra!=0) {tot_bloques+=1; hay_sobra=true;}
	 
	 if (tot_bloques>24) {
		 cout<<"No hay suficientes enlaces para indexar su archivo.."<<endl;
		 return;
	 }	 
	 
	 int indx, ev1,ev2;
	 bloque blk;
	 int cont1=0, cont2=0, cont3=0; //para ei
	 bool primera1=true, primera2=true;;
	 for (int i=0; i<tot_bloques; i++){ //completo los 'cant_bloques' bloques de datos		  
		 
		 if(sobra!=0 && i==cant_bloques){	
			 limpiar(aux1);
			 fread(&aux1[0],1,sobra,archivo);			 
		 }else{		 
			fread(&aux1[0], 1, tam_bl,archivo);
		 }
		 blk.b=aux1;
		 blk.ocupado=true;
			 if(datos.empty()){ //si el vector de datos esta vacio			 		 
				 datos.push_back(blk);			 
			}else{			 
				 indx= buscar_sig("bloque"); //busca el siguiente bloque de datos vacio			 			 
				 datos[indx]=blk;
				 cout<<"Bloque: "<<indx<<endl;
			} 		  
			
			///****************enlaces del inodo*****************						 
			
			 //esto se hace "cant_bloques" veces.				 
			 
			 int t= in.enlaces.size();
			 
			 //enlaces directos
			 
			 if (t<4) {
				 in.enlaces.push_back(indx);	 
				 
			 } 
			 
			 //enlaces indirectos simples
			 else if ( (t==4 || t==5) && cont1<4 ){ //si (use todos los enlaces directos O arranque a usar los is) Y no use 4 ) ....
				 //con el cont me aseguro que no uso mas de 4 enlaces.
				 
				 if (primera1){ //actualizo enlaces del ->inodo<- solamente(*)
				//esto es porque, si bien uso hasta 4 enlaces indirectos simples, en mi inodo , solo necesito el indice del 
				//"bloque" que contiene esos 4 enlaces indirectos					 
					 ev1=buscar_sig("enlace"); //enlace vacio
					 in.enlaces.push_back(ev1); //guardo el indice del enlace vacio (el que contiene el vector de 4)					 
					 primera1=false;
				 }
				 enlaces[ev1].push_back(indx); //guardo el indice del bloque ocupado en la lista de enlaces is
				 
				 cont1++;					 
			 }
			 
			 //enlaces indirectos dobles			 
			 else if ( (t==5 && cont1==4) || (t==6) ){ 
				 //si  (arranque a usar enlaces is Y ya use los 4) O (arranque a usar los enlaces id Y no use los 4)...				 
				 
				if (primera2){ //(*)					 
					 ev1=buscar_sig("enlace"); //enlace vacio
					 in.enlaces.push_back(ev1); //relleno en enlace indirecto doble del inodo
					 
					 primera2=false;					 					 
					 ev2=buscar_sig("enlace++"); //segundo enlace vacio
				}				 	
				
				if(sobra!=0 && i==cant_bloques){enlaces[ev1].push_back(ev2);}
				if (cont2<4){	
					if(cont3<4){
						enlaces[ev2].push_back(indx);
					}else{
						enlaces[ev1].push_back(ev2); //guardo antes de pasar al otro (3) -> mejor aca :D
						cont2++;
						cont3=0;
						ev2=buscar_sig("enlace");
						enlaces[ev2].push_back(indx);
					}
					cont3++;
				}else{
					cout<<"No hay mas lugar para guardar..."<<endl;
					break; //si use los 4 enlaces de mi unico enlace indirecto doble disponible, entonces no tengo mas
					//lugares donde guardar.
				}
			 }
			///**************************************************	
			 
	}

	 
} //guardar
