/*! \file Constantes.h
 * \brief Archivo de cabecera para la clase Constantes.
 * Esta clase no tiene métodos, sólo datos. Por ahora sólo contiene los
 * códigos globales de los ciclos y las fases.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef _CONSTANTES_H
#define _CONSTANTES_H

#include <string>

class Constantes
{
 public:
 	//Paso de tiempo Ciclo
 	static constexpr double dt_difusion = 0.01;
 	static constexpr double dt_mecanica = 0.1;
 	static constexpr double dt_ciclo = 6.0;

	// Modelos de ciclos
	static const int ciclo_Ki67= 0;
	static const int ciclo_vida= 1;
    static const int hepato_30= 2;
    static const int hepato_70= 3;
    static const int ciclo_Ki67_cancer= 4;
    

	//Modelos de ciclos de muerte
	static const int ciclo_de_muerte_necrosis = 100;
	static const int ciclo_de_muerte_apoptosis = 101;

	// Fases de los ciclos
	static const int Ki67_positiva_premitotica=0;
	static const int Ki67_positiva_postmitotica=1;
	static const int Ki67_negativa=2;
	static const int viva=3;
    static const int G0i=4;
    static const int Agrandar=5;
    static const int Achicar=6;
    static const int G0f=7;

	//Fases de las muertes
	static const int necrotica_hinchada = 100;
	static const int necrotica_lisada = 101;
	static const int necrotica = 102;
	static const int debris = 104;
	static const int apoptotica = 105;


};


#endif
