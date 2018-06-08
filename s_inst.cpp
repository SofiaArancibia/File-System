/**@brief Funcion que guarda 'palabras' en un arreglo
Cada palabra es un término de la instrucción. Luego, en este arreglo se busca lo necesario. Por ejemplo
el primer elemento será el comando ('cd', 'ls',...). Luego seguirán los argumentos de dicho comando.
Se podrá buscar, por ejemplo, el caracter '|' (pipe) para conectar comandos.
Si se encuentra, la palabra inmediata siguiente será otro comando. 
 @author Arancibia, Sofia Casandra
 @date 03/07/2015
*/ 


#include <cstring>

using namespace std;

void s_inst(char *inst,char *ar[], char op){	
	if (op=='e'){
	int i=0;
	char *p; //para strtok
		//strtok divide una oracion en palabras (tokens)
		//char * strtok ( char * str, const char * delimiters );
		p = strtok(inst," ");
		ar[i]=p;		
			while(p != NULL){//al llegar al final de la oracion strtok retorna null.
				i++;
				//la primera llamada recibe la cadena, las siguientes usan la cadena que resta luego de la primer llamada.
				//eso se indica pasandole null al primer arguemento de strtok.
				p = strtok(NULL, " ");
				//se agrega la palabra al arreglo.
				ar[i]=p;				
			}
			i++; 
//			ar[i]='\0';
	}else if(op=='b'){
		int i=0;
		char *p; //para strtok
		//strtok divide una oracion en palabras (tokens)
		//char * strtok ( char * str, const char * delimiters );
		p = strtok(inst,"/");
		ar[i]=p;		
		while(p != NULL){//al llegar al final de la oracion strtok retorna null.
			i++;
			//la primera llamada recibe la cadena, las siguientes usan la cadena que resta luego de la primer llamada.
			//eso se indica pasandole null al primer arguemento de strtok.
			p = strtok(NULL, "/");
			//se agrega la palabra al arreglo.
			ar[i]=p;				
		}
		i++; 
		ar[i]='\0';
	}
	
}


//Referencias:
//http://www.cplusplus.com/reference/cstring/strtok/

	
