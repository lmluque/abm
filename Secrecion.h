#ifndef __SECRECION_H__
#define __SECRECION_H__

//#include "Microambiente.h"
#include <vector>

class Secrecion
{
 private:
 public:

	std::vector<double> tasas_de_secrecion;
	std::vector<double> tasas_de_consumo;
	std::vector<double> densidades_de_saturacion;
	std::vector<double> tasas_de_exportacion_neta;
	double oncoproteina;

	Secrecion(); 

	void avanzar( double dt );

	void set_todas_las_secreciones_a_cero( void ); 
	void set_todos_los_consumos_a_cero( void );
	void multiplicar_las_secreciones_por_un_factor( double factor );
	void multiplicar_los_consumos_por_un_factor( double factor );
};

#endif
