/*! \file Constantes.h
 * \brief Archivo de cabecera para la clase Constantes.
 * Esta clase no tiene métodos, sólo datos. Por ahora sólo contiene los
 * códigos globales de los ciclos y las fases.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#ifndef _MACROS_H
#define _MACROS_H

#include <vector>
#include <cmath>
#include <string>

#define g_indice_de_voxel(i,j,k) (k*coordenadas_y.size() + j )*coordenadas_x.size() + i


#define fg_indice_de_voxel(i,j,k) (k*mgrilla.coordenadas_y.size() + j )*mgrilla.coordenadas_x.size() + i
        
      
#endif
