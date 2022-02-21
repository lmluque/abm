/*! \file Random.cpp
 * \brief Archivo de implementación de la clase RNG
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1.0
 * \date 2020.06.09
 */
#include "Random.h"

RNG::RNG() : n_max(1.0), seed(13), media(1.0), std_desv(0.25) {}

/*
RNG::RNG(double NM, int SD) : n_max(NM), seed(SD),
    dist(std::uniform_real_distribution<double>(0.0, NM)) {
        generator.seed(seed);
}
*/
RNG::RNG(double NM, int SD, double med, double desv) : n_max(NM), seed(SD), media(med), std_desv(desv), 
dist(std::uniform_real_distribution<double>(0.0, NM)), d(std::normal_distribution<double>(media,std_desv)) {
        generator.seed(seed);
}


double RNG::RandomNumber() {
    return dist(generator);
};

double RNG::NormalRandom(){
    return d(generator);
}


double RNG::NormalRandom_CM( double mean, double standard_deviation )
{
	std::normal_distribution<double> d2(mean,standard_deviation);
	return d2(generator);
}

double RNG::RandomNumber( double min, double max )
{
	std::uniform_real_distribution<double> dist2(min,max);
	return dist2(generator);
}
