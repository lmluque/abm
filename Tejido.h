#ifndef __TEJIDO_H__
#define __TEJIDO_H__

#include "Random.h"
#include "Microambiente.h"
#include "Contenedor_de_Celulas.h"
#include "Parametros_globales.h"

extern RNG *rng;
extern Parametros_globales *pg;
extern std::vector<Celula*> todas_las_celulas;
extern std::vector<Celula*> celulas_listas_para_dividirse;
extern std::vector<Celula*> celulas_para_registrar_en_voxeles;

//Microambiente microambiente;
//Microambiente_Parametros p;


class Tejido{
	public:

	Microambiente microambiente;
//	Microambiente_Parametros p;
	Contenedor_de_Celulas cdc;
    double radio_del_tumor;
    double volumen_del_tumor;
    double volumen_del_tumor2;
    int celulas_tumorales;
    int celulas_muertas;

	Tejido();

	void inicializar_tejido();
	std::vector<Vector> crear_planos_de_celulas_sanas_en_Z(int cantidad, double posicion_en_z_del_primer_plano);
	std::vector<Vector> crear_esfera_de_celulas(double radio_de_la_esfera);
	void introducir_linfocitos(int cantidad);
    void introducir_linfocitos_aleatorios(int cantidad);
    void geometria_del_tumor();
    void cortar_higado(int PHx);
    std::vector<Vector> crear_plano_de_vs();
    void controlar_vasos_sanguineos();
};


#endif

