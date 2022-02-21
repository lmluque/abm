/*! \file Fase.cpp 
 * \brief Archivo de implementación de la clase Fase.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#include "Fase.h"

Fase::Fase(){
	
	indice = 0;
	codigo = 0;
	nombre = "Sin nombre";
	funcion_de_entrada = NULL;
	
	division_al_final_de_la_fase = false;
	remover_al_final_de_la_fase = false;
	actualizar_volumen = false;
	
	return;	
	
}
