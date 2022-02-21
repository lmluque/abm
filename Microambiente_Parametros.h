#ifndef _MICROAMBIENTE_PARAMETROS_H
#define _MICROAMBIENTE_PARAMETROS_H


#include <vector>
#include <string>

class Microambiente_Parametros
{
 private:
 
 public: 
	//Microambiente* pMicroambiente;
	std::string nombre; 
 
	std::string unidades_espaciales; 
	std::string unidades_temporales;
	double dx;
	double dy; 
	double dz; 
	
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
	
	//Microambiente_Parametros(); 
	
	bool calcular_gradientes; 
	
	bool usar_oxigeno_como_primer_sustrato;
    
};

#endif
