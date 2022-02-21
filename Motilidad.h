#ifndef __MOTILIDAD_H__
#define __MOTILIDAD_H__


#include "Vector.h"
#include<vector>

extern Vector *v;

class Motilidad{
 public:
	bool es_movil;

	double tiempo_de_persistencia; 
	double velocidad_de_migracion; 
	Vector bias_de_la_migracion_direccion; 
	double bias_de_la_migracion; 
	Vector vector_de_motilidad;

	int quimiotaxis_indice;
	int quimiotaxis_direccion;

	Motilidad(); 


};

#endif
