#include "Volumen.h"


class Geometria{
	public:
	double radio;
	double radio_nuclear;
	double area_superficial;
	double polaridad;

	Geometria(); 
	void actualizar_radio(Volumen& volumen);
	void actualizar_radio_nuclear(Volumen& volumen);
	void actualizar_area_superficial(Volumen& volumen);
	void actualizar(Volumen& volumen);
};

