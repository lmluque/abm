/*! \file Ciclo_Modelo.cpp
 * \brief Archivo de implementación de la clase Ciclo_Modelo.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */

#include "Ciclo_Modelo.h"



Ciclo_Modelo::Ciclo_Modelo(){

	mapas_de_indice_inverso.resize(0);

	nombre = "Sin nombre";
	unidades_tiempo = "min";
	codigo = 9999;

	fases.resize(0);
	fase_links.resize(0);
	tasas_de_transicion.resize(0);


	return;

}

int Ciclo_Modelo::agregar_fase( int codigo, std::string nombre  ){

	int n = fases.size();

	fases.resize(n+1);
	fase_links.resize(n+1);
	tasas_de_transicion.resize(n+1);
	fase_links[n].resize(0);
	tasas_de_transicion[n].resize(0);

	mapas_de_indice_inverso.resize(n+1);
	mapas_de_indice_inverso[n].clear();

	fases[n].codigo = codigo;
	fases[n].indice = n;
	fases[n].nombre.assign( nombre );

	return n;

}

int Ciclo_Modelo::agregar_link(int indice_fase_inicial, int indice_fase_final, bool (*funcion_arrest)( Volumen& volumen, Muerte_parametros& mp ) ){


	int n =	fase_links[indice_fase_inicial].size();
	fase_links[indice_fase_inicial].resize(n+1);
	tasas_de_transicion[indice_fase_inicial].resize(n+1);



	fase_links[indice_fase_inicial][n].indice_fase_inicial = indice_fase_inicial;
	fase_links[indice_fase_inicial][n].indice_fase_final = indice_fase_final;
	fase_links[indice_fase_inicial][n].funcion_arrest = funcion_arrest;

	mapas_de_indice_inverso[indice_fase_inicial][indice_fase_final] = n;


	return n;

}

double& Ciclo_Modelo::tasa_de_transicion( int indice_fase_inicial, int indice_fase_final){

	return tasas_de_transicion[indice_fase_inicial][mapas_de_indice_inverso[indice_fase_inicial][indice_fase_final]];

}

double Ciclo_Modelo::get_tasa_de_transicion( int indice_fase_inicial){

	double tasa;
	tasa = 0.0;
	for (unsigned int k=0; k < fase_links[indice_fase_inicial].size(); k++){
		tasa = tasas_de_transicion[indice_fase_inicial][k];
	}
	return tasa;

}

bool Ciclo_Modelo::get_actualizar_volumen(int indice_fase_inicial){

	return fases[indice_fase_inicial].actualizar_volumen;
}

Fase_Link& Ciclo_Modelo::fase_link(int indice_fase_inicial, int indice_fase_final){

	return fase_links[indice_fase_inicial][mapas_de_indice_inverso[indice_fase_inicial][indice_fase_final]];

}

std::ostream& Ciclo_Modelo::mostrar_ciclo( std::ostream& os ){

	os << "Ciclo Celular: " << nombre << "(Codigo: " << codigo << ")" << std::endl;
	os << "Fases y Links: (el * denota division celular en esta fase)" << std::endl;
	for(unsigned int i=0; i< fases.size(); i++){

		os << "La fase " << i << " (" << fases[i].nombre << ") ";

		if(fases[i].division_al_final_de_la_fase){
			os << " * ";
		}
		os << "se conecta con: " << std::endl;
		for( unsigned int k=0 ; k < fase_links[i].size() ; k++ )
		{
			int j = fase_links[i][k].indice_fase_final;

			os << "\tla fase " << j << " (" << fases[j].nombre << ") con tasa de transicion " << tasa_de_transicion(i,j) << " " << unidades_tiempo << "^-1;" << std::endl;
		}
		os << std::endl;
	}
	return os;
}


void Ciclo_Modelo::avanzar_en_el_modelo(bool& flagged_para_remover, bool& flagged_para_dividirse, int& indice_de_la_fase_actual, double& tiempo_acumulado_en_la_fase, Volumen& volumen, double dt,std::vector< std::vector<double> >& c_tasas_de_transicion, Muerte_parametros& mp){
	//std::cout << "AVANZAR EN EL MODELO \n";
	int i = indice_de_la_fase_actual;
	tiempo_acumulado_en_la_fase += dt;

	int j;
	for ( unsigned int k=0; k < fase_links[i].size(); k++){
		j = fase_links[i][k].indice_fase_final;

		bool transicion_parada = false;
		if(fase_links[i][k].funcion_arrest){
			//std::cout << "Primer if \n";
			transicion_parada = fase_links[i][k].funcion_arrest(volumen, mp);
		}

		if(!transicion_parada){
			//std::cout << "segundo if \n";

			bool continuar_transicion = false;
			if(fase_links[i][k].duracion_fija){
				//std::cout << "tercer if \n";
				if(tiempo_acumulado_en_la_fase > 1.0 / c_tasas_de_transicion[i][k]){
					continuar_transicion = true;
				}
			}
			else{

				double prob = c_tasas_de_transicion[i][k]*dt;

				if(rng->RandomNumber() <= prob){
					continuar_transicion = true;
				}
			}

			if (continuar_transicion){
					//std::cout << "cuarto if \n";
				if(fases[i].division_al_final_de_la_fase){
					//std::cout << "Se divide al final de la fase \n";
					//volumen.dividir();
					flagged_para_dividirse = true;
				}

				if(fases[i].remover_al_final_de_la_fase){
						//std::cout << "remover final de la fase \n";
					flagged_para_remover = true;
					return;
				}

				indice_de_la_fase_actual = j;
				tiempo_acumulado_en_la_fase = 0.0;

				if(fases[j].funcion_de_entrada){
					fases[j].funcion_de_entrada(volumen, mp);
				}

				return;
			}
		}
	}

	//std::cin.get();

	return;

}

int Ciclo_Modelo::encontrar_indice_de_la_fase(int codigo){

	for( unsigned int i=0 ; i < fases.size() ; i++ )
	{
		if( fases[i].codigo == codigo )
		{ return i; }
	}
	return 0;

}


int Ciclo_Modelo::get_indice_de_mapa_inverso(int fase_uno, int fase_dos){

    return mapas_de_indice_inverso[fase_uno][fase_dos];

}
