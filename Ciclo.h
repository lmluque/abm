/*! \file Ciclo.h
 * \brief Archivo de cabecera para la clase Ciclo.
 *  La célula va a tener una instancia de ciclo, la cual le da un puntero
 *  que apunta en la dirección de alguna instacia de Ciclo_Modelo.
 *  Cuando nace, la madre le pasa su puntero ya que las dos siguen el mismo
 *  ciclo. Sin embargo, ante cualquier factor externo, esta puede cambiar su
 *  ciclo. Es decir, apuntar su puntero en la dirección de otra instancia de
 *  Ciclo_Modelo.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef __CICLO_H__
#define __CICLO_H__


#include "Ciclos_estandares.h"


class Ciclo{

	public:
	Ciclo_Modelo* pCiclo_Modelo;
	int indice_de_la_fase_actual;
	double tiempo_acumulado_en_la_fase;
	bool flagged_para_dividirse;
	bool flagged_para_remover;
	std::vector< std::vector<double> > tasas_de_transicion;
		
	/** Constructor
    * Inicializa el constructor con:
    * pCiclo_Modelo = NULL,
    */
	Ciclo();
	
	/** \fn sync_con_ciclo_modelo
    * \brief Le dice a la célula a qué ciclo mirar, apuntando su puntero en
    * la dirección de alguna instancia de Ciclo_Modelo.
    * \param Ciclo_Modelo& cm : El ciclo que va a seguir la célula.
    * \return void
    */
	void sync_con_ciclo_modelo( Ciclo_Modelo& cm );
	
	//void avanzar_en_el_ciclo( int& indice_de_la_fase_actual, double& tiempo_acumulado_en_la_fase, Volumen& volumen, double dt ); // done
	void avanzar_en_el_ciclo( Volumen& volumen, double dt, std::vector< std::vector<double> >& c_tasas_de_transicion, Muerte_parametros& mp );
	
	bool actualizar_volumen();
	
	double tasa_de_transicion();
    
    double& actualizar_mis_tasas_de_transicion(int fase_actual, int fase_siguiente);
	
	Fase& fase_actual();
};

#endif
