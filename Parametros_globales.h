#ifndef __PARAMETROS_GLOBALES_H__
#define __PARAMETROS_GLOBALES_H__

#include "Ciclo_Modelo.h"
#include <vector>
#include <string>


class Parametros_globales{

	public:
    //Seed para los numeros aleatorios
    int seed;

	//Microambiente
	std::string m_nombre;

	std::string unidades_espaciales;
	std::string unidades_temporales;
	double m_dx;
	double m_dy;
	double m_dz;

    //Condiciones de Periodicidad
	bool condiciones_de_periodicidad;
	bool condiciones_de_periodicidad_x;
	bool condiciones_de_periodicidad_y;
	bool condiciones_de_periodicidad_z;

	bool condiciones_de_Dirichlet_externas;
	std::vector<double> vector_condicion_de_dirichlet;
	std::vector<bool> vector_activacion_dirichlet;

	std::vector<bool> dirichlet_todo;

	std::vector<bool> dirichlet_xmin;
	std::vector<bool> dirichlet_xmax;
	std::vector<bool> dirichlet_ymin;
	std::vector<bool> dirichlet_ymax;
	std::vector<bool> dirichlet_zmin;
	std::vector<bool> dirichlet_zmax;
	std::vector<bool> dirichlet_vs;

	std::vector<double> dirichlet_xmin_valores;
	std::vector<double> dirichlet_xmax_valores;
	std::vector<double> dirichlet_ymin_valores;
	std::vector<double> dirichlet_ymax_valores;
	std::vector<double> dirichlet_zmin_valores;
	std::vector<double> dirichlet_zmax_valores;

	std::vector<double> vector_condiciones_iniciales;

	std::vector<double> rango_en_X;
	std::vector<double> rango_en_Y;
	std::vector<double> rango_en_Z;

	bool calcular_gradientes;

	bool usar_oxigeno_como_primer_sustrato;


	//CONTENEDOR
	double c_dx;
	double c_dy;
	double c_dz;


	//CELULA
	Ciclo_Modelo ciclo;
	double tasas_de_secrecion;
	double tasas_de_consumo;
	double densidades_de_saturacion;
	std::string c_nombre;
	int tipo;
	double o2_saturacion_para_la_proliferacion;
	double o2_referencia;
	bool interactuar_con_mb;
	bool crecer_al_costado;
	int numero_id;

	void set_parametros(std::string, std::string);

	//COSAS INMUNES
	double imm_mean;
	double imm_sd;
	bool activar_respuesta_inmune;
    int cantidad_de_linfocitos;
    double tiempo_de_imm;
    double tiempo_de_imm_2;

    double tiempo_total;
	double tiempo_final;
    
    //Cosas de tejido sano
    double probabilidad;
    int divisiones_permitidas;
    bool activar_sustancia_x;
    bool hepatectomia;

};

extern Parametros_globales *pg;

#endif
