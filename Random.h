/*! \file Random.h
 * \brief Archivo de cabecera para la clase RNG
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1.0
 * \date 2020.06.09
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <iostream>
#include <random>

class RNG {
    private:
        double n_max;
        int seed;
        double media;
        double std_desv;
        std::mt19937 generator;
        std::uniform_real_distribution<double> dist;
        std::uniform_real_distribution<double> dist2;
        std::normal_distribution<double> d;
        std::normal_distribution<double> d2;

    public:
        /** Constructor
         * Inicializa el constructor con parámetros por defecto:
         * n_max = 1.0, seed = 13
         */
        RNG();

        /** Constructor
         * \param double NM : límite superior de la distribución.
         * \param int SD : semilla del generador.
         */
        RNG(double NM, int SD, double med, double desv);

        /** \fn RandomNumber
         * \brief Devuelve un número aleatorio en [0.0, NM).
         * \return double
         */
        double RandomNumber();
        double NormalRandom();
        double NormalRandom_CM( double mean, double standard_deviation );
        double RandomNumber( double min, double max );
        //double NormalRandom( double mean, double standard_deviation );
};


#endif
