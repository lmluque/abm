#include "Muerte.h"

Muerte::Muerte(){

	tasas.resize(0);
	ciclos.resize(0);
	parametros.resize(0);

	muerta = false;
	indice_del_ciclo_de_muerte_actual= 0;

	return;

}

int Muerte::agregar_ciclo_de_muerte(double tasa, Ciclo_Modelo* pModelo){

	tasas.push_back(tasa);
	ciclos.push_back(pModelo);
	parametros.resize(tasas.size());

	return tasas.size() - 1;

}

int Muerte::agregar_ciclo_de_muerte(double tasa, Ciclo_Modelo* pModelo, Muerte_parametros& muerte_parametros){

	tasas.push_back(tasa);
	ciclos.push_back(pModelo);
	parametros.push_back(muerte_parametros);

	return tasas.size() - 1;

}

int Muerte::encontrar_indice_del_ciclo_de_muerte(int codigo){

	for( unsigned int i=0 ; i < ciclos.size() ; i++ ){

		if( ciclos[i]->codigo == codigo ){
			return i;
		}
	}
	return 0;

}

int Muerte::encontrar_indice_del_ciclo_de_muerte(std::string nombre){

	for( unsigned int i=0 ; i < ciclos.size() ; i++ ){

		if( ciclos[i]->nombre == nombre ){
			return i;
		}
	}
	return 0;

}

bool Muerte::chequear_muerte(double dt){
	//std::cout << "chequear muerte \n";
	if( muerta == true )
	{//std::cout << "primer if \n";
		return false;
	}

	unsigned int i = 0;
	//std::cout << "aleatorio = " << rng->RandomNumber() << " \n";
	//std::cout << "tasas size = " << tasas.size() << " \n";
	while( !muerta && i < tasas.size() )
	{//std::cout << "while \n";
	//std::cout << "aleatorio = " << rng->RandomNumber() << " \n";
	//std::cout << "tasas = " << tasas[i]*dt << " \n";
	double numaleat = rng->RandomNumber();
		if( numaleat < tasas[i]*dt )
		{//std::cout << "segundo if \n";
			muerta = true;
			indice_del_ciclo_de_muerte_actual = i;
            //std::cout << numaleat << "\n";
            //std::cin.get();

			return muerta;
		}
		i++;
		//std::cout << "tasas = " << tasas.size() << " i = " << i << "\n";
	}

	//std::cout << "sali del while \n";
	return muerta;

}

void Muerte::comenzar_muerte(int indice_ciclo_de_muerte){

	muerta = true;
	indice_del_ciclo_de_muerte_actual = indice_ciclo_de_muerte;
	return;

}

Ciclo_Modelo& Muerte::ciclo_actual(void){

	return *ciclos[indice_del_ciclo_de_muerte_actual];

}

Muerte_parametros& Muerte::parametros_actuales(void){

	return parametros[indice_del_ciclo_de_muerte_actual];
}
