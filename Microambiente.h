#ifndef __MICROAMBIENTE_H__
#define __MICROAMBIENTE_H__

//#include "Contenedor_de_Celulas.h"
#include "Grillado.h"
#include "Parametros_globales.h"

extern Vector *v;
extern Parametros_globales *pg;
//extern Microambiente_Parametros *p;

typedef std::vector<double> gradiente;

class Microambiente{

	private:
		std::vector< std::vector<double> > temp_vectores_densidad1;
		std::vector< std::vector<double> > temp_vectores_densidad2;

		std::vector< std::vector<double> > temp_bulk_fuente_sumidero_solver1;
		std::vector< std::vector<double> > temp_bulk_fuente_sumidero_solver2;
		std::vector< std::vector<double> > temp_bulk_fuente_sumidero_solver3;
		bool setup_del_solver_bulk_fuente_sumidero_hecho;

		std::vector< std::vector<double> >* p_vectores_densidad;

		std::vector< std::vector<gradiente> > vectores_gradiente;
		std::vector<bool> vector_gradiente_calculado;

		std::vector<double> uno;
		std::vector<double> cero;
		std::vector<double> un_medio;
		std::vector<double> un_tercio;

		std::vector< std::vector<double> > thomas_temp1;
		std::vector< std::vector<double> > thomas_temp2;
		std::vector<double> thomas_constante1x;
		std::vector<double> thomas_constante1y;
		std::vector<double> thomas_constante1z;
		std::vector<double> thomas_neg_constante1x;
		std::vector<double> thomas_neg_constante1y;
		std::vector<double> thomas_neg_constante1z;
		bool setup_de_thomas_hecho;
		int thomas_salto_en_i;
		int thomas_salto_en_j;
		int thomas_salto_en_k;
		std::vector<double> thomas_constante1;
		std::vector<double> thomas_constante1a;
		std::vector<double> thomas_constante2;
		std::vector<double> thomas_constante3;
		std::vector<double> thomas_constante3a;
		std::vector< std::vector<double> > thomas_denomx;
		std::vector< std::vector<double> > thomas_cx;
		std::vector< std::vector<double> > thomas_denomy;
		std::vector< std::vector<double> > thomas_cy;
		std::vector< std::vector<double> > thomas_denomz;
		std::vector< std::vector<double> > thomas_cz;
		bool setup_del_solver_de_difusion_hecho;

		std::vector< std::vector<double> > vector_valores_de_dirichlet;
		std::vector<bool> vector_activacion_dirichlet;
		std::vector< std::vector<bool> > dirichlet_vectorES_activacion;

	public:


		Grillado_Cartesiano mgrilla;

		std::string unidades_espaciales;
		std::string unidades_temporales;
		std::string nombre;

		std::vector< std::string > densidades_nombres;
		std::vector< std::string > densidades_unidades;

		std::vector< double > coeficientes_de_difusion;
		std::vector< double > tasas_de_decaimiento;

		std::vector< std::vector<double> > densidades_objetivo_de_suministro_por_tasas_de_suministro;
		std::vector< std::vector<double> > tasas_de_suministro;
		std::vector< std::vector<double> > tasas_de_consumo;

		std::vector<int> voxeles_del_vaso_sanguineo;
        std::vector<int> posibles_voxeles_del_vaso_sanguineo;

		void actualizar_tasas( void );

		Microambiente();

		unsigned int numero_de_densidades( void );
		unsigned int numero_de_voxeles( void );


		void redimensionar_espacio( double x_ini, double x_fin, double y_ini, double y_fin, double z_ini, double z_fin , double dx_nuevo , double dy_nuevo , double dz_nuevo );

		void redimensionar_densidades( int nuevo_tamano );
		void agregar_densidad( void );
		void agregar_densidad (std::string nombre , std::string unidades, double coeficiente_de_difusion, double tasa_de_decaimiento );

		void set_densidad(int indice, std::string nombre , std::string unidades , double coeficiente_de_difusion , double tasa_de_decaimiento );
		void set_densidad(int indice, std::string nombre , std::string unidades );

		int encontrar_indice_de_densidad(std::string nombre);
		int indice_de_voxel( int i, int j, int k );
        int indice_de_voxel( Vector& indices_cartesianos ); //Le paso los indices cartesianos en un Vector
		std::vector<unsigned int> indices_cartesianos( int n );

		int indice_del_voxel_mas_cercano( Vector& posicion);
		Vector centro_del_voxel(int indice_del_voxel);
		Vector indices_cartesianos_mas_cercanos(Vector& posicion );
		Voxel& voxel_mas_cercano( Vector& posicion );
		Voxel& voxeles( int indice_de_voxel );
		std::vector<double>& vector_de_densidades_mas_cercano( Vector& posicion );
		std::vector<double>& vector_de_densidades_mas_cercano( int indice_de_voxel );

		std::vector<double>& operator()( int i, int j, int k );
		std::vector<double>& operator()( int n );
		std::vector<double>& vector_de_densidades( int i, int j, int k );
		std::vector<double>& vector_de_densidades( int n );


		std::vector<gradiente>& vector_de_gradientes(int i, int j, int k);
		std::vector<gradiente>& vector_de_gradientes(int n );

		std::vector<gradiente>& vector_de_gradiente_mas_cercano( Vector& posicion );

		void calcular_todos_los_vectores_de_gradientes( void );
		void calcular_vector_de_gradiente( int n );
		void resetear_todos_los_vectores_de_gradientes( void );

		void simular_difusion_decaimiento( double dt );

		void simular_suministro_y_consumo_del_bulk( double dt );

		void simular_suministro_y_consumo_de_las_celulas( double dt );


		void mostrar_informacion( std::ostream& os );


		void agregar_nodo_de_dirichlet( int indice_de_voxel, std::vector<double>& valor );
		void actualizar_nodo_de_dirichlet( int indice_de_voxel , std::vector<double>& nuevo_valor );
		void actualizar_nodo_de_dirichlet( int indice_de_voxel , int indice_de_sustrato, double nuevo_valor );
		void remover_nodo_de_dirichlet( int indice_de_voxel );
		void aplicar_condiciones_de_dirichlet( void );

		void set_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato , bool nuevo_valor );

		void set_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato , int indice, bool nuevo_valor );
		void set_activacion_de_sustrato_de_dirichlet( int indice, std::vector<bool>& nuevo_valor );
		bool get_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato, int indice );


		bool& es_nodo_de_dirichlet( int indice_de_voxel );

		void solver_decaimiento_de_la_difusion__coeficientes_constantes_LOD_3D( double dt );

		void inicializar_microambiente();

		void crear_vaso_sanguineo(int xmin, int ymin, int zmin, int xmax, int ymax, int zmax);
        void posibles_nodos(std::vector<Vector>& vector_posiciones);
        void vasos_del_higado(double radio, std::vector<Vector>& indices);
        


};

void set_microambiente_default( Microambiente* M );
Microambiente* get_microambiente_default( void );


#endif
