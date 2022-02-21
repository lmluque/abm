#include "Parametros_globales.h"

#include <string>


void Parametros_globales::set_parametros(std::string parametro, std::string valor){
	std::string valorstr;
	double valord;
	bool valorb;
	int valori;
	bool interp = false;
	Ciclo_Modelo Ki67, necrosis;
//	int seed para los numeros aleatorios;
		if (parametro ==  "seed") {
			valori = std::stoi(valor);
			pg->seed = valori;
			interp = true; }    
//	std::string m_nombre;
		if (parametro ==  "m_nombre") {
			valorstr = valor;
			pg->m_nombre = valorstr;
			interp = true; }
//	std::string unidades_espaciales;
		if (parametro ==  "unidades_espaciales") {
			valorstr = valor;
			pg->unidades_espaciales = valorstr;
			interp = true; }
//	std::string unidades_temporales;
		if (parametro ==  "unidades_temporales") {
			valorstr = valor;
			pg->unidades_temporales = valorstr;
			interp = true; }
//	double m_dx;
		if (parametro ==  "m_dx") {
			valord = std::stod(valor);
			pg->m_dx = valord;
			interp = true; }
//	double m_dy;
		if (parametro ==  "m_dy") {
			valord = std::stod(valor);
			pg->m_dy= valord;
			interp = true; }
//	double m_dz;
		if (parametro ==  "m_dz") {
			valord = std::stod(valor);
			pg->m_dz = valord;
			interp = true; }
//	bool condiciones_de_periodicidad;
		if (parametro ==  "condiciones_de_periodicidad") {
			valorb = (valor == "true")? true : false;
			pg->condiciones_de_periodicidad = valorb;
			interp = true; }
//	bool condiciones_de_periodicidad_x;
		if (parametro ==  "condiciones_de_periodicidad_x") {
			valorb = (valor == "true")? true : false;
			pg->condiciones_de_periodicidad_x = valorb;
			interp = true; }
//	bool condiciones_de_periodicidad_y;
		if (parametro ==  "condiciones_de_periodicidad_y") {
			valorb = (valor == "true")? true : false;
			pg->condiciones_de_periodicidad_y = valorb;
			interp = true; }
//	bool condiciones_de_periodicidad_z;
		if (parametro ==  "condiciones_de_periodicidad_z") {
			valorb = (valor == "true")? true : false;
			pg->condiciones_de_periodicidad_z = valorb;
			interp = true; }
//	bool condiciones_de_Dirichlet_externas;
		if (parametro ==  "condiciones_de_Dirichlet_externas") {
			valorb = (valor == "true")? true : false;
			pg->condiciones_de_Dirichlet_externas = valorb;
			interp = true; }
//	std::vector<double> vector_condicion_de_dirichlet;
		if (parametro ==  "vector_condicion_de_dirichlet") {
			valord = std::stod(valor);
			pg->vector_condicion_de_dirichlet.push_back(valord);
			interp = true; }
//	std::vector<bool> vector_activacion_dirichlet;
		if (parametro ==  "vector_activacion_dirichlet") {
			valorb = (valor == "true")? true : false;
			pg->vector_activacion_dirichlet.push_back(valorb);
			interp = true; }
//	std::vector<bool> dirichlet_todo;
		if (parametro ==  "dirichlet_todo") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_todo.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_xmin;
		if (parametro ==  "dirichlet_xmin") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_xmin.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_xmax;
		if (parametro ==  "dirichlet_xmax") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_xmax.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_ymin;
		if (parametro ==  "dirichlet_ymin") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_ymin.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_ymax;
		if (parametro ==  "dirichlet_ymax") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_ymax.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_zmin;
		if (parametro ==  "dirichlet_zmin") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_zmin.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_zmax;
		if (parametro ==  "dirichlet_zmax") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_zmax.assign( 1 , valorb );
			interp = true; }
//	std::vector<bool> dirichlet_vs;
		if (parametro ==  "dirichlet_vs") {
			valorb = (valor == "true")? true : false;
			pg->dirichlet_vs.assign( 1 , valorb );
			interp = true; }
//	std::vector<double> dirichlet_xmin_valores;
		if (parametro ==  "dirichlet_xmin_valores") {
			valord = std::stod(valor);
			pg->dirichlet_xmin_valores.push_back(valord);
			interp = true; }
//	std::vector<double> dirichlet_xmax_valores;
		if (parametro ==  "dirichlet_xmax_valores") {
			valord = std::stod(valor);
			pg->dirichlet_xmax_valores.push_back(valord);
			interp = true; }
//	std::vector<double> dirichlet_ymin_valores;
		if (parametro ==  "dirichlet_ymin_valores") {
			valord = std::stod(valor);
			pg->dirichlet_ymin_valores.push_back(valord);
			interp = true; }
//	std::vector<double> dirichlet_ymax_valores;
		if (parametro ==  "dirichlet_ymax_valores") {
			valord = std::stod(valor);
			pg->dirichlet_ymax_valores.push_back(valord) ;
			interp = true; }
//	std::vector<double> dirichlet_zmin_valores;
		if (parametro ==  "dirichlet_zmin_valores") {
			valord = std::stod(valor);
			pg->dirichlet_zmin_valores.push_back(valord);
			interp = true; }
//	std::vector<double> dirichlet_zmax_valores;
		if (parametro ==  "dirichlet_zmax_valores") {
			valord = std::stod(valor);
			pg->dirichlet_zmax_valores.push_back(valord);
			interp = true; }
//	std::vector<double> vector_condiciones_iniciales;
		if (parametro ==  "vector_condiciones_iniciales") {
			valord = std::stod(valor);
			pg->vector_condiciones_iniciales.push_back(valord);
			interp = true; }
//	std::vector<double> rango_en_X;
		if (parametro ==  "rango_en_X") {
			valord = std::stod(valor);
			pg->rango_en_X.push_back(valord);
			interp = true; }
//	std::vector<double> rango_en_Y;
		if (parametro ==  "rango_en_Y") {
			valord = std::stod(valor);
			pg->rango_en_Y.push_back(valord);
			interp = true; }
//	std::vector<double> rango_en_Z;
		if (parametro ==  "rango_en_Z") {
			valord = std::stod(valor);
			pg->rango_en_Z.push_back(valord);
			interp = true; }
//	bool calcular_gradientes;
		if (parametro ==  "calcular_gradientes") {
			valorb = (valor == "true")? true : false;
			pg->calcular_gradientes = valorb;
			interp = true; }
//	bool usar_oxigeno_como_primer_sustrato;
		if (parametro ==  "usar_oxigeno_como_primer_sustrato") {
			valorb = (valor == "true")? true : false;
			pg->usar_oxigeno_como_primer_sustrato = valorb;
			interp = true; }
//	double c_dx;
		if (parametro ==  "c_dx") {
			valord = std::stod(valor);
			pg->c_dx = valord;
			interp = true; }
//	double c_dy;
		if (parametro ==  "c_dy") {
			valord = std::stod(valor);
			pg->c_dy= valord;
			interp = true; }
//	double c_dz;
		if (parametro ==  "c_dz") {
			valord = std::stod(valor);
			pg->c_dz = valord;
			interp = true; }
//	Ciclo_Modelo ciclo;
		if (parametro ==  "ciclo") {
			if (valor == "Ki67") {
				pg->ciclo = Ki67;
			} else {
				pg->ciclo = necrosis;
			}
			interp = true; }
//	double tasas_de_secrecion;
		if (parametro ==  "tasas_de_secrecion") {
			valord = std::stod(valor);
			pg->tasas_de_secrecion = valord;
			interp = true; }
//	double tasas_de_consumo;
		if (parametro ==  "tasas_de_consumo") {
			valord = std::stod(valor);
			pg->tasas_de_consumo = valord;
			interp = true; }
//	double densidades_de_saturacion;
		if (parametro ==  "densidades_de_saturacion") {
			valord = std::stod(valor);
			pg->densidades_de_saturacion = valord;
			interp = true; }
//	std::string c_nombre;
		if (parametro ==  "c_nombre") {
			valorstr = valor;
			pg->c_nombre = valorstr;
			interp = true; }
//	int tipo;
		if (parametro ==  "tipo") {
			valori = std::stoi(valor);
			pg->tipo = valori;
			interp = true; }
//	double o2_saturacion_para_la_proliferacion;
		if (parametro ==  "o2_saturacion_para_la_proliferacion") {
			valord = std::stod(valor);
			pg->o2_saturacion_para_la_proliferacion = valord;
			interp = true; }
//	double o2_referencia;
		if (parametro ==  "o2_referencia") {
			valord = std::stod(valor);
			pg->o2_referencia = valord;
			interp = true; }
//	bool interactuar_con_mb;
		if (parametro ==  "interactuar_con_mb") {
			valorb = (valor == "true")? true : false;
			pg->interactuar_con_mb = valorb;
			interp = true; }
//	bool crecer_al_costado;
		if (parametro ==  "crecer_al_costado") {
			valorb = (valor == "true")? true : false;
			pg->crecer_al_costado = valorb;
			interp = true; }
// static double imm_mean
		if (parametro ==  "imm_mean") {
			valord = std::stod(valor);
			pg->imm_mean = valord;
			interp = true; }
// static double imm_sd
		if (parametro ==  "imm_sd") {
			valord = std::stod(valor);
			pg->imm_sd = valord;
			interp = true; }
// bool activar_respuesta_inmune
		if (parametro ==  "activar_respuesta_inmune") {
			valorb = (valor == "true")? true : false;
			pg->activar_respuesta_inmune = valorb;
			interp = true; }
//	int cantidad de linfocitos a inyectar;
		if (parametro ==  "cantidad_de_linfocitos") {
			valori = std::stoi(valor);
			pg->cantidad_de_linfocitos = valori;
			interp = true; }
//	double tiempo de inyección de la inmunoterapia;
		if (parametro ==  "tiempo_de_imm") {
			valord = std::stod(valor);
			pg->tiempo_de_imm = valord;
			interp = true; }
//	double tiempo de inyección de la 2 dosis de inmunoterapia;
		if (parametro ==  "tiempo_de_imm_2") {
			valord = std::stod(valor);
			pg->tiempo_de_imm_2 = valord;
			interp = true; }
//	double probabilidad de transición celular;
		if (parametro ==  "probabilidad") {
			valord = std::stod(valor);
			pg->probabilidad = valord;
			interp = true; }
//	int cantidad de divisiones permitidas;
		if (parametro ==  "divisiones_permitidas") {
			valori = std::stoi(valor);
			pg->divisiones_permitidas = valori;
			interp = true; }			
// bool activar_respuesta_inmune
		if (parametro ==  "activar_sustancia_x") {
			valorb = (valor == "true")? true : false;
			pg->activar_sustancia_x = valorb;
			interp = true; }
			
			if (interp == false ) {
			std::cout << "nombre de parametro erroneo \n";
			std::cout << "pulse enter para continuar \n";
			std::cin.get();
		}
}

