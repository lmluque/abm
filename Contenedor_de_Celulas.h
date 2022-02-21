#ifndef __CONTENEDOR_DE_CELULAS_H__
#define __CONTENEDOR_DE_CELULAS_H__

#include "Grillado.h"
#include "Celula.h"
#include <vector>
#include <fstream>



class Contenedor_de_Celulas{
	private:

	public:
		Grillado_Cartesiano grillado;
		std::vector<std::vector<Celula*> > celulas_en_voxel;
		std::vector<std::vector<Celula*> > celulas_fuera_del_domino;
		int num_de_divisiones_en_este_paso;
		int num_de_muertes_en_este_paso;
		int num_de_celulas;
        double tiempo_desde_la_ultima_mecanica;
        double hora_de_la_ultima_mecanica;
		Celula* celula;
		
		Contenedor_de_Celulas();
		
		//void inicializar(double x_ini, double x_fin, double y_ini, double y_fin, double z_ini, double z_fin , double voxel_size);
		void inicializar(double x_ini, double x_fin, double y_ini, double y_fin, double z_ini, double z_fin , double dx, double dy, double dz);
		void registrar_celula( Celula* celula );
		void agregar_celula_a_voxel(Celula* celula, int indice_de_voxel);
		void actualizar_todas_las_celulas(double tiempo_total, double dt_difusion, double dt_mecanico, double dt_ciclo);
		bool contiene_alguna_celula(int indice_de_voxel);
		void sacar_celula_de_voxel(Celula* celula, int indice_de_voxel);
		void actualizar_voxeles_de_celulas();
		void agregar_potenciales_cdc(Celula* celula, Celula* otra_celula);
};

extern std::vector<Celula*> todas_las_celulas;
extern std::vector<Celula*> celulas_listas_para_dividirse;
extern std::vector<Celula*> celulas_listas_para_remover;
extern std::vector<Celula*> celulas_para_registrar_en_voxeles;

#endif
