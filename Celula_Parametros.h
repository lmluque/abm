#ifndef _CELULA_PARAMETROS_H
#define _CELULA_PARAMETROS_H

#include "Ciclo_Modelo.h"
#include <vector>
#include <string>

class Celula_Parametros{
	
	public:
		Ciclo_Modelo ciclo;
		double tasas_de_secrecion; 
		double tasas_de_consumo; 
		double densidades_de_saturacion;
		std::string nombre;
		int tipo;
		double o2_saturacion_para_la_proliferacion;
		double o2_referencia;
	
};

#endif
