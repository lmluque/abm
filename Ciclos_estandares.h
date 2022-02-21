/*! \file Ciclos_estandares.h
 * \brief Archivo de cabecera para Ciclos_estandares.cpp.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */


#include "Constantes.h"
#include "Ciclo_Modelo.h"
#include "Muerte_Parametros.h"



extern Ciclo_Modelo Ki67, vida, hepato_30, hepato_70, ki67_cancer, necrosis, apoptosis;
extern Muerte_parametros necrosis_parametros, apoptosis_parametros;
extern bool ciclo_celular_estandar_inicializado;
extern bool ciclo_celular_de_muerte_inicializado;

// standard entry function for the cycle models
void Ki67_fase_positiva_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp); // done
void fase_viva_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp);
void hepato30_agrandar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp);
void hepato30_achicar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp);
bool achicar_G0i_funcion_de_arrest(Volumen& volumen, Muerte_parametros& mp);
void hepato70_agrandar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp);
void hepato70_achicar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp);

void standard_necrosis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp );  // done
void standard_apoptosis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp );
void standard_lysis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp ); // done
bool standard_necrosis_funcion_de_arrest( Volumen& volumen, Muerte_parametros& mp ); // done

/** \fn crear_ciclo_celular_estandar
* \brief Construye los ciclos celulares que se van a usar a lo largo
*  del programa. Estos son instancias de Ciclo_Modelo. Se construyen una
*  sola vez y según las condiciones de la célula, va a mirar a alguno de
*  ellos. Ahora hay uno solo, pero dependiendo de la cantidad de células
*  distintas y de muertes que estas puedan experimentar, va a haber más.
*  Yo calculo, por lo menos 4.
*/
bool crear_ciclo_celular_estandar( void );
bool crear_ciclo_de_muerte_estandar( void );
