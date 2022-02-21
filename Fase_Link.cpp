/*! \file Fase_Link.cpp 
 * \brief Archivo de implementación de la clase Fase_Link.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */


#include "Fase_Link.h"

Fase_Link::Fase_Link(){
	
	indice_fase_inicial = 0;
	indice_fase_final = 0; 
	duracion_fija = false;
	
	funcion_arrest = NULL;
	
	return;
}
