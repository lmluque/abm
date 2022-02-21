#ifndef __GRILLADO_H__
#define __GRILLADO_H__

#include <iostream>
#include "Macros.h"
#include "Voxel.h"
#include "Parametros_globales.h"

extern Parametros_globales *pg;

class Grillado_General
{
 private:
	friend std::ostream& operator<<(std::ostream& os, const Grillado_General& grillado);

 public:
	Grillado_General();

	// [xmin ymin zmin xmax ymax zmax ]
	std::vector<double> caja;
	std::vector<Voxel> voxeles;

	std::vector< std::vector<int> > indices_de_voxeles_conectados;

	int indice_del_voxel_mas_cercano( Vector& posicion );
	bool es_valida_la_posicion(double x, double y, double z);

	void conectar_voxeles(int i,int j);
	std::string unidades;
	void mostrar_informacion_general( std::ostream& os);
};


class Grillado_Cartesiano : public Grillado_General
{
 private:

 public:
	std::vector<double> coordenadas_x;
	std::vector<double> coordenadas_y;
	std::vector<double> coordenadas_z;
	std::vector< std::vector<int> > indices_de_voxeles_conectados_tipo_moore; 
	void crear_vecindario_moore(void);
	void crear_vecindario_moore_optimizado(void);    
	void crear_vecindario_moore_periodico(void);
    void crear_vecindario_moore_periodico_optimizado();
	unsigned int indice_de_voxel( unsigned int i, unsigned int j, unsigned int k );
	std::vector<unsigned int> indices_cartesianos( unsigned int n );

	double dx;
	double dy;
	double dz;

	double dV;
	double dS;

	double dS_xy;
	double dS_yz;
	double dS_xz;

	Grillado_Cartesiano();

	Grillado_Cartesiano( int , int , int );


	void redimensionar( int,int,int );
	void redimensionar( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , int x_nodes, int y_nodes, int z_nodes );
	void redimensionar( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx, double dy, double dz );

	int indice_del_voxel_mas_cercano( Vector& posicion );
	Vector indices_cartesianos_mas_cercanos(Vector& posicion );
	Voxel& voxel_mas_cercano( Vector& posicion );
	Vector get_centro_voxel(int indice_de_voxel);

	void mostrar_informacion_cartesiano( std::ostream& os );
	void mostrar_vecindarios_moore(std::ostream& os);

};


#endif
