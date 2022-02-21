#include "Motilidad.h"

Motilidad::Motilidad()
{
	es_movil = false;

	tiempo_de_persistencia = 1.0;
	velocidad_de_migracion = 1.0;

	bias_de_la_migracion_direccion.x=0.0;
	bias_de_la_migracion_direccion.y=0.0;
	bias_de_la_migracion_direccion.z=0.0;
	bias_de_la_migracion = 0.0;

	vector_de_motilidad.x=0.0;
	vector_de_motilidad.y=0.0;
	vector_de_motilidad.z=0.0;

	quimiotaxis_indice = 0;
	quimiotaxis_direccion = 1;

	return;
}
