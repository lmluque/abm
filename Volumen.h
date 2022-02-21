#ifndef __VOLUMEN_H__
#define __VOLUMEN_H__

#include <math.h> 

class Volumen{
	public:

        double ideal;
	    double total;
	    double solido;
	    double fluido;
	    double fraccion_de_fluido;
	    
	    double nuclear;
	    double nuclear_fluido;
	    double nuclear_solido;
	    
	    double citoplasmatico;
	    double citoplasmatico_fluido;
	    double citoplasmatico_solido;
	    
	    double fraccion_calcificada;
	    
	    double relacion_citoplasma_nucleo;
	    
	    double volumen_de_ruptura;
	    
	    double citoplasma_tasa_de_cambio;
	    double nucleo_tasa_de_cambio;
	    double fluido_tasa_de_cambio;
	    
	    double tasa_de_calcificacion;
	    
	    double target_citoplasma_solido;
	    double target_nucleo_solido;
	    double target_fraccion_fluido;
	    double target_relacion_citoplasma_nucleo;
	    
	    double volumen_de_ruptura_relativo;
	    
	    //double tasa_de_cambio; 
	    //double target;
	    bool cambio_el_volumen;
	    
	    Volumen();
	    void dividir(void);
	    void multiplicar(double);
};

#endif
