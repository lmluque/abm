#ifndef __CELULA_H__
#define __CELULA_H__

#include <fstream>
#include "Fenotipo.h"
#include "Vector.h"
#include "Random.h"
#include "Microambiente.h"
#include "Parametros_globales.h"
#include "Parametros.h"
#include "Motilidad.h"


extern RNG *rng;
extern Vector *v;
extern Constantes *c;
extern Parametros_globales *pg;
extern Ciclo_Modelo Ki67, vida, necrosis, apoptosis;
extern Muerte_parametros necrosis_parametros, apoptosis_parametros;
//extern Celula_Parametros *cp;
//extern Microambiente *pmicroambiente

class Celula{
	private:
		Microambiente *microambiente;

	protected:
		std::vector<double> temp_celula_fuente_sumidero_solver1;
		std::vector<double> temp_celula_fuente_sumidero_solver2;
		std::vector<double> temp_celula_fuente_sumidero_exportacion_solver1;
		std::vector<double> temp_celula_fuente_sumidero_exportacion_solver2;
		Vector velocidad_anterior;
		bool es_activa;

	public:
		int voxel_del_microambiente;
		int voxel;
		std::string nombre;
	    int tipo;
		int id;
	    int indice;
	    int madre;
	    double tiempo_desde_el_ultimo_ciclo;
	    double tiempo_desde_la_ultima_mecanica;
	    double hora_del_ultimo_ciclo;
	    double hora_de_la_ultima_mecanica;
	    double hora_global;
	    double tasa;
	    bool adherida;
	    Fenotipo fenotipo;
	    Parametros parametros;
	    Vector posicion;
    	Vector velocidad;
    	Vector desplazamiento;
        Celula* celula_adherida;
// Para el tejido sano:
        double probabilidad;
        double probabilidad_aleat;
        int divisiones_permitidas;
        int divisiones;


		Celula();
		void actualizar_volumen( Fenotipo& fenotipo, double dt);
		void actualizar_parametros_de_celula_y_muerte_con_o2(Fenotipo& fenotipo, double dt);
		void actualizar_parametros_de_celula_y_muerte_con_o2_y_oncoproteina(Fenotipo& fenotipo, double dt);
		void actualizar_parametros_de_celula_y_muerte_con_o2_y_x(Fenotipo& fenotipo, double dt);
        void avanzar_funciones_del_fenotipo(double hora_global, double dt_ciclo);
		void avanzar_funciones_del_fenotipo_con_O2(double hora_global, double dt_ciclo);
		void avanzar_funciones_del_fenotipo_con_O2_y_oncoproteina(double hora_global, double dt_ciclo);
        void avanzar_funciones_del_fenotipo_con_O2_y_x(double hora_global, double dt_ciclo);
		//		void flag_division();
		Celula* dividir();
		void morir(int indice);
//		bool set_posicion(Vector nueva_posicion);
		bool set_posicion(double x, double y ,double z);
		bool set_posicion(Vector posicion);
		void agregar_potenciales(Celula* otra_celula);
		void agregar_potenciales_mb();
		void agregar_potenciales_mb_2();
		void actualizar_posicion( double dt );
		void comenzar_muerte( int indice_del_ciclo_de_muerte );
//		void set_volumen(double);
//		void set_volumen_target(double);
//		void set_volumen_target_con_radio(double);
//		void set_volumen_con_radio(double);
//		double& get_volumen();
//		void copiar_data(Celula*);
//		void copiar_punteros_a_funciones(Celula*);

		/*Parte del consumo de OXÍGENO y demáses*/
		void registrar_microambiente( Microambiente* );
		void actualizar_voxel_del_microambiente();
		void set_constantes_de_consumo_interno( double dt ); 
		Microambiente* get_microambiente( void );
		void simular_secrecion_y_consumo( double dt );
		int get_indice_del_voxel_del_microambiente();
		std::vector<double>& vector_de_densidades_mas_cercano( void );
		std::vector<double>& gradiente_mas_cercano( int substrate_index );


		/////PARA INICIAR LA CELULA
		void inicializar_celula();
		void inicializar_celula_sana();
        void inicializar_hepato();
		void mostrar_informacion_de_la_celula(std::ostream& os);

		//Para el linfocito...Polimorfismo
        virtual void es_movil(bool valor) {return;};
		//virtual void inicializar_linfocito() {return;};
		virtual void actualizar_vector_de_motilidad( double dt, std::vector<Celula*> celulas_en_mi_voxel ) {return;};
		virtual void avanzar_linfocito( double dt, std::vector<Celula*> celulas_en_mi_voxel ) {return;};

};

class Linfocito: public Celula{

    public:
        Motilidad motilidad;
        double tasa_de_asesinato;
        double tiempo_de_adhesion;
        double tasa_de_adhesion;
        double constante_elastica;
        double distancia_de_adhesion_maxima;
        double distancia_de_adhesion_minima;
        double saturacion_de_oncoproteina;
        double limite_de_oncoproteina;
        double diferencia_de_oncoproteina;
        double diferencia_de_adhesion;
//        Celula* celula_adherida;

        //using Celula::Celula;
        Linfocito();
        //void inicializar_linfocito();
        void es_movil(bool valor);
        void actualizar_vector_de_motilidad( double dt, std::vector<Celula*> celulas_en_mi_voxel );
        void motilidad_de_linfocito( double dt, std::vector<Celula*> celulas_en_mi_voxel  );
        void avanzar_linfocito( double dt, std::vector<Celula*> celulas_en_mi_voxel );
        bool intento_de_apoptosis( Celula* celula_adherida, double dt );
        bool desencadenar_apoptosis( Celula* celula_adherida );
        void adherir_celula(Celula* celula_objetivo);
        void soltar_celula(Celula* celula_objetivo);
        bool chequear_vecinos_para_adherirse( std::vector<Celula*> celulas_en_mi_voxel , double dt );
        bool intentar_adherirse( Celula* celula_objetivo , double dt );

};

extern std::vector<Celula*> todas_las_celulas;
extern std::vector<Celula*> celulas_listas_para_dividirse;
extern std::vector<Celula*> celulas_listas_para_remover;
extern std::vector<Celula*> celulas_para_registrar_en_voxeles;
Celula* crear_celula();
Linfocito* crear_linfocito();

#endif
