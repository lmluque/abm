#include "Muerte_Parametros.h"

Muerte_parametros::Muerte_parametros(){
	
		tiempo_unidades = "min";
		
		tasa_de_cambio_fluido_no_lisado = 3.0/60.0;
		tasa_de_cambio_fluido_lisado = 0.05/60.0;
		
		citoplasma_tasa_de_cambio = 1.0/60.0;
	    nucleo_tasa_de_cambio = 0.35/60.0;
	    
	    tasa_de_calcificacion = 0.0;
	    
	    volumen_de_ruptura_relativo = 2.0;

	
}
