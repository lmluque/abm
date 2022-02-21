#ifndef __MUERTE_PARAMETROS_H__
#define __MUERTE_PARAMETROS_H__

#include <string>

class Muerte_parametros{
	
	public:
		std::string tiempo_unidades;
		
		double tasa_de_cambio_fluido_no_lisado;
		double tasa_de_cambio_fluido_lisado;
		
		double citoplasma_tasa_de_cambio;
	    double nucleo_tasa_de_cambio;
	    
	    double tasa_de_calcificacion;
	    
	    double volumen_de_ruptura_relativo;
	    
	    Muerte_parametros();
	
};


#endif
