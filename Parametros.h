#ifndef __PARAMETROS_H__
#define __PARAMETROS_H__

#include <string>


//#include "Fenotipo.h"

class Parametros{

	public:

	double o2_saturacion_para_la_proliferacion; 
	double o2_limite_de_proliferacion;
	double o2_referencia;
	double o2_necrosis_limite; 
	double o2_necrosis_max; 
	
	double X_saturacion_para_la_proliferacion; 
	double X_limite_de_proliferacion; 

	double tasa_necrosis_max; 
	
	double x_corte30;

	Parametros();

};

#endif
