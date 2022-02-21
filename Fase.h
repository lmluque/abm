/*! \file Fase.h
 * \brief Archivo de cabecera para la clase Fase.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef __FASE_H__
#define __FASE_H__

#include <string>
#include "Geometria.h"
#include "Muerte_Parametros.h"

class Fase{
	public:
		int indice;
		int codigo;
		std::string nombre;
		bool division_al_final_de_la_fase;
		bool remover_al_final_de_la_fase;
		bool actualizar_volumen;
		void (*funcion_de_entrada)(Volumen& volumen, Muerte_parametros& mp);
		
		/** Constructor
    	* Inicializa el constructor con parámetros por defecto:
    	* indice = 0,
		* codigo = 0,
		* nombre = "Sin nombre",
		* division_al_final_de_la_fase = false,
	    */
		Fase();

};

#endif
