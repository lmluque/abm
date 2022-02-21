#include "Parametros.h"

#include <string>


Parametros::Parametros(){


	o2_necrosis_limite = 0.0;
	o2_necrosis_max = 0.0;

	o2_limite_de_proliferacion = 0.0;
	o2_saturacion_para_la_proliferacion = 38.0; 
	o2_referencia = 38.0; //38.0; 

	X_limite_de_proliferacion = 0.25;
	X_saturacion_para_la_proliferacion = 0.50; 
	
	
	tasa_necrosis_max = 1.0 / (6.0 * 60.0); 
    
    x_corte30 = 0.72;


	return;


}
