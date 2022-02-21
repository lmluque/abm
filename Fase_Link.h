/*! \file Fase_Link.h
 * \brief Archivo de cabecera para la clase Fase_Link.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef __FASE_LINK_H__
#define __FASE_LINK_H__

#include "Volumen.h"
#include "Muerte_Parametros.h"


class Fase_Link{
	public:
		int indice_fase_inicial;
		int indice_fase_final;
		bool duracion_fija;
		
		bool (*funcion_arrest)( Volumen& volumen, Muerte_parametros& mp );
		
		/** Constructor
    	* Inicializa el constructor con parámetros por defecto:
    	* indice_fase_inicial = 0,
    	* indice_fase_final = 0, 
    	* duracion_fija = false,
	    */
		Fase_Link();
	
};


#endif
