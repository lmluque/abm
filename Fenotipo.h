#ifndef __FENOTIPO_H__
#define __FENOTIPO_H__

#include "Ciclo.h"
#include "Mecanica.h"
#include "Secrecion.h"
//#include "Molecular.h"
#include "Muerte.h"

class Fenotipo
{
 private:
 public:
	 
	Ciclo ciclo; 
	Volumen volumen; 
	Geometria geometria;
	Mecanica mecanica;
	Secrecion secrecion;
//	Molecular molecular;
	Muerte muerte;

	Fenotipo(); // done 
	
};

#endif
