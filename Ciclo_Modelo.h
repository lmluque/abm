/*! \file Ciclo_Modelo.h
 * \brief Archivo de cabecera para la clase Ciclo_Modelo.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef __CICLO_MODELO_H__
#define __CICLO_MODELO_H__

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>

//#include "Geometria.h"
#include "Fase.h"
#include "Fase_Link.h"
#include "Random.h"

//#include "Volumen.h"

extern RNG *rng;


class Ciclo;

class Ciclo_Modelo{
	private:
		std::vector< std::unordered_map<int,int> > mapas_de_indice_inverso; 
 
 	public:
 		std::string nombre;
		std::string unidades_tiempo;
 		int codigo; 
 		std::vector<Fase> fases; 
 		std::vector< std::vector<Fase_Link> > fase_links; 
		std::vector< std::vector<double> > tasas_de_transicion;

		/** Constructor
    	* Inicializa el constructor con parámetros por defecto:
    	* 	mapas_de_indice_inverso.resize(0),
    	* 	nombre = "Sin nombre",
    	* 	unidades_tiempo = "min",
    	* 	codigo = 9999,
    	* 	indice_de_la_fase_actual = 0,
    	*	fases.resize(0),
    	*	fase_links.resize(0),
    	*	tasas_de_transicion.resize(0),
    	*	tiempo_acumulado_en_la_fase = 0,
    	*/
		Ciclo_Modelo(); 
		
		/** \fn agregar_fase
		* \brief Agrega las fases al grafo del ciclo
		* \param int codigo : Código global de la Fase.
		* \param std::string nombre : Nombre de la Fase
    	* \return int
    	*/
		int agregar_fase( int codigo, std::string nombre);

		/** \fn agregar_link
		* \brief Agrega los links al grafo del ciclo
		* \param int indice_fase_inicial : Indice de la fase inicial
		* (No confundir índice con código global)
		* \param int indice_fase_final : Indice de la fase final
		* (No confundir índice con código global)
    	* \return int
    	*/
		int agregar_link(int indice_fase_inicial, int indice_fase_final, bool (*funcion_arrest)( Volumen& volumen, Muerte_parametros& mp ) );

		/** \fn tasa_de_transicion
		* \brief Me devuelve por referencia las tasas de transición usando los
		* índices de las fases inicial y final. El vector tasas_de_transicion 
		* tiene la misma estructura que fase_links, por lo que las funciones
		* para obtener sus valores son iguales.
		* IMPORTANTE: No confundir el vector tasas_de_transicion con la función
		* tasa_de_transicion. Tienen nombres muy parecidos. 
		* \param int indice_fase_inicial : Indice de la fase inicial
		* (No confundir índice con código global)
		* \param int indice_fase_final : Indice de la fase final
		* (No confundir índice con código global)
    	* \return double&
    	*/
		double& tasa_de_transicion( int indice_fase_inicial, int indice_fase_final);

		/** \fn fase_link
		* \brief Me devuelve por referencia los links entre fases usando los
		* índices de las fases inicial y final. El vector fase_links tiene la
		* misma estructura que tasas_de_transicion, por lo que las funciones
		* para obtener sus valores son iguales.
		* IMPORTANTE: No confundir el vector fase_links con la función
		* fase_link. Tienen nombres muy parecidos. 
		* \param int indice_fase_inicial : Indice de la fase inicial
		* (No confundir índice con código global)
		* \param int indice_fase_final : Indice de la fase final
		* (No confundir índice con código global)
    	* \return Fase_Link&
    	*/
		Fase_Link& fase_link(int indice_fase_inicial, int indice_fase_final);

		/** \fn mostrar_ciclo
		* \brief Me escribe el ciclo en la terminal.
		* \param std::ostream& os : Yo uso y recomiendo std::cout 
    	* \return std::ostream&
    	*/
	    std::ostream& mostrar_ciclo( std::ostream& os );
	    
	    void avanzar_en_el_modelo(bool& flagged_para_remover, bool& flagged_para_dividirse, int& indice_de_la_fase_actual, double& tiempo_acumulado_en_la_fase, Volumen& volumen, double dt, std::vector< std::vector<double> >& c_tasas_de_transicion, Muerte_parametros& mp); // done 
	    
	    double get_tasa_de_transicion( int indice_fase_inicial);
	    
	    bool get_actualizar_volumen(int indice_fase_inicial);
	    
	    int encontrar_indice_de_la_fase(int codigo);
        
        int get_indice_de_mapa_inverso(int fase_uno, int fase_dos);
};

#endif
