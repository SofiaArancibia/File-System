#ifndef BLOQUE_H
#define BLOQUE_H

class bloque{
public:
	bool ocupado;
	//bloque de datos de 1k
	vector<unsigned char> b; 
	void clear() {b.clear();};
	
	bloque(){
		b=vector<unsigned char>(1024);
	}
};

#endif
