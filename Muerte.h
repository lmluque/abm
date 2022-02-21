#ifndef __MUERTE_H__
#define __MUERTE_H__

#include<vector>
#include "Ciclo_Modelo.h"
#include "Muerte_Parametros.h"
#include "Random.h"

extern RNG *rng;

class Muerte{
	
	public:
		std::vector<double> tasas;
		std::vector<Ciclo_Modelo*> ciclos;
		std::vector<Muerte_parametros> parametros;
		
		bool muerta;
		int indice_del_ciclo_de_muerte_actual;
		
		Muerte();
		
		int agregar_ciclo_de_muerte(double tasa, Ciclo_Modelo* pModelo);
		int agregar_ciclo_de_muerte(double tasa, Ciclo_Modelo* pModelo, Muerte_parametros& muerte_parametros);
		
		int encontrar_indice_del_ciclo_de_muerte(int codigo);
		int encontrar_indice_del_ciclo_de_muerte(std::string nombre);
		
		bool chequear_muerte(double dt);
		void comenzar_muerte(int indice_ciclo_de_muerte);
		
		Ciclo_Modelo& ciclo_actual(void);
		Muerte_parametros& parametros_actuales(void);
	
	
	
};


#endif
