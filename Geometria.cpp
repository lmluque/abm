#include <math.h>
#include "Geometria.h"

Geometria::Geometria(){

	radio = 8.412710547954228;
	radio_nuclear = 5.051670902881889;
	area_superficial = 889.3685284131693;

	polaridad = 0.0;
	return;
}

void Geometria::actualizar_radio(Volumen& volumen){

	static double cuatro_tercios_de_pi = 4.188790204786391;
	radio = volumen.total;
	radio /= cuatro_tercios_de_pi;
	radio = pow( radio, 0.333333333333333333333333333333333333333);
	return;

}

void Geometria::actualizar_radio_nuclear(Volumen& volumen){

	static double cuatro_tercios_de_pi = 4.188790204786391;
	radio_nuclear = volumen.nuclear;
	radio_nuclear /= cuatro_tercios_de_pi;
	radio_nuclear = pow( radio_nuclear, 0.333333333333333333333333333333333333333);
	return;

}

void Geometria::actualizar_area_superficial(Volumen& volumen){

	static double constante = 4.835975862049409;
	area_superficial = pow( volumen.total , 0.666666666666667 );
	area_superficial /= constante;

	return;

}

void Geometria::actualizar(Volumen& volumen){

	actualizar_radio(volumen);
	actualizar_radio_nuclear(volumen);

	area_superficial = volumen.total;
	area_superficial /= radio;
	area_superficial *= 3.0;

	return;
}

