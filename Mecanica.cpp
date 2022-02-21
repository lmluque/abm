#include "Mecanica.h"

Mecanica::Mecanica()
{
	fuerza_de_adhesion_cc = 0.4;
	fuerza_de_repulsion_cc = 10.0;
    fuerza_de_adhesion_co = 0.0;
    fuerza_de_repulsion_co = 10.0;
 	fuerza_de_adhesion_mb = 4.0;
	fuerza_de_repulsion_mb = 10.0;
	distancia_de_adhesion_maxima_relativa = 1.25;

	return;
}
