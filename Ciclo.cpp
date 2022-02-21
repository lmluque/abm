/*! \file Ciclo.cpp
 * \brief Archivo de implementación de la clase Ciclo.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */


#include "Ciclo.h"
#include <cstddef>

Ciclo::Ciclo(){

	pCiclo_Modelo = NULL;
	indice_de_la_fase_actual = 0;
	tiempo_acumulado_en_la_fase = 0;
	flagged_para_dividirse= false;
	flagged_para_remover= false;

	return;
}

void Ciclo::sync_con_ciclo_modelo( Ciclo_Modelo& cm ){

	pCiclo_Modelo = &cm;
	tasas_de_transicion = cm.tasas_de_transicion;
	return;

}



void Ciclo::avanzar_en_el_ciclo(Volumen& volumen, double dt, std::vector< std::vector<double> >& c_tasas_de_transicion, Muerte_parametros& mp ){

	pCiclo_Modelo->avanzar_en_el_modelo(flagged_para_remover, flagged_para_dividirse, indice_de_la_fase_actual, tiempo_acumulado_en_la_fase, volumen, dt, c_tasas_de_transicion, mp);
	return;
}

bool Ciclo::actualizar_volumen(){

	return pCiclo_Modelo->get_actualizar_volumen(indice_de_la_fase_actual);

}


double Ciclo::tasa_de_transicion(){

	double tasa;
	tasa = pCiclo_Modelo->get_tasa_de_transicion(indice_de_la_fase_actual);
	return tasa;
}

double& Ciclo::actualizar_mis_tasas_de_transicion(int fase_actual, int fase_siguiente){

    return tasas_de_transicion[fase_actual][pCiclo_Modelo->get_indice_de_mapa_inverso(fase_actual,fase_siguiente)];


}


Fase& Ciclo::fase_actual(){

	return pCiclo_Modelo->fases[indice_de_la_fase_actual];

}

