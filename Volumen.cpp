#include "Volumen.h"

Volumen::Volumen(){
	
	fraccion_de_fluido=0.75;
    ideal = 2494.0;
	total=2494.0;
	fluido = fraccion_de_fluido * total;
	solido = total- fluido;
	nuclear=540.0;
	nuclear_fluido = fraccion_de_fluido * nuclear;
	nuclear_solido = nuclear - nuclear_fluido;
	citoplasmatico = total - nuclear;
    citoplasmatico_fluido = fraccion_de_fluido * citoplasmatico;
	citoplasmatico_solido = citoplasmatico - citoplasmatico_fluido;
	fraccion_calcificada = 0.0;
	relacion_citoplasma_nucleo = citoplasmatico / ( 1e-16 + nuclear);
	/*//Tasas para corte 30%    
	citoplasma_tasa_de_cambio = 0.090/60.0;
	nucleo_tasa_de_cambio = 0.11238/60.0;
	fluido_tasa_de_cambio = 0.90/60.0;
	*/
    citoplasma_tasa_de_cambio = 0.27/60.0;//0.26/60.0;
	nucleo_tasa_de_cambio = 0.33/60.0;//0.213980/60.0;
	fluido_tasa_de_cambio = 3.0/60.0;//2.6/60.0;
	tasa_de_calcificacion = 0.0;
	target_citoplasma_solido = citoplasmatico_solido;
	target_nucleo_solido = nuclear_solido;
	target_fraccion_fluido = fraccion_de_fluido;
	target_relacion_citoplasma_nucleo = relacion_citoplasma_nucleo;
	volumen_de_ruptura_relativo = 2.0;
	volumen_de_ruptura = volumen_de_ruptura_relativo * total;
	cambio_el_volumen = true;	    
	
	return;
}

void Volumen::multiplicar(double numero){
	
	total *= numero;
	solido *= numero;
	fluido *= numero;
	
	nuclear *= numero;
	nuclear_fluido *= numero;
	nuclear_solido *= numero;
	    
	citoplasmatico *= numero;
	citoplasmatico_fluido *= numero;
	citoplasmatico_solido *= numero;
	    
	volumen_de_ruptura *= numero;
	    
	target_citoplasma_solido *= numero;
	target_nucleo_solido *= numero;
	
	
	
	return;
	
}

void Volumen::dividir(void){
	
	multiplicar(0.5);
	return;
}

