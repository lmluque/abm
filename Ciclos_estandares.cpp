/*! \file Ciclos_estandares.cpp
 * \brief Acá voy a crear los modelos de ciclos estandares que
 * se van a usar a lo largo del programa. Por ahora hay uno solo,
 * pero va a haber más.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1936519365913.0
 * \date 2020.07.17
 */


#include "Ciclos_estandares.h"

void Ki67_fase_positiva_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp)
{
	volumen.target_nucleo_solido *= 2.0;
	volumen.target_citoplasma_solido *= 2.0;
	return;
}

void fase_viva_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp){

	volumen.target_nucleo_solido *= 2.0;
	volumen.target_citoplasma_solido *= 2.0;

	return;

}

void standard_apoptosis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp )
{
	volumen.target_fraccion_fluido = 0.0;
	volumen.target_citoplasma_solido = 0.0;
	volumen.target_nucleo_solido = 0.0;

	volumen.target_relacion_citoplasma_nucleo = 0.0;


	volumen.citoplasma_tasa_de_cambio = mp.citoplasma_tasa_de_cambio;

	volumen.nucleo_tasa_de_cambio = mp.nucleo_tasa_de_cambio;

	volumen.fluido_tasa_de_cambio = mp.tasa_de_cambio_fluido_lisado;

	volumen.tasa_de_calcificacion = mp.tasa_de_calcificacion;

	volumen.volumen_de_ruptura_relativo = mp.volumen_de_ruptura_relativo;
	volumen.volumen_de_ruptura = volumen.total * volumen.volumen_de_ruptura_relativo;

	return;
}

void standard_necrosis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp ){

	volumen.target_fraccion_fluido = 1.0;
	volumen.target_citoplasma_solido = 0.0;
	volumen.target_nucleo_solido = 0.0;

volumen.target_relacion_citoplasma_nucleo = 0.0;

	volumen.citoplasma_tasa_de_cambio = mp.citoplasma_tasa_de_cambio;

	volumen.nucleo_tasa_de_cambio = mp.nucleo_tasa_de_cambio;

	volumen.fluido_tasa_de_cambio = mp.tasa_de_cambio_fluido_lisado;

	volumen.tasa_de_calcificacion = mp.tasa_de_calcificacion;

	volumen.volumen_de_ruptura_relativo = mp.volumen_de_ruptura_relativo;

	volumen.volumen_de_ruptura =
	volumen.total * volumen.volumen_de_ruptura_relativo;

	return;

}

void standard_lysis_funcion_de_entrada( Volumen& volumen, Muerte_parametros& mp ){

	volumen.target_fraccion_fluido = 0.0;
	volumen.target_citoplasma_solido = 0.0;
	volumen.target_nucleo_solido = 0.0;

	volumen.citoplasma_tasa_de_cambio = mp.citoplasma_tasa_de_cambio;

	volumen.nucleo_tasa_de_cambio = mp.nucleo_tasa_de_cambio;

	volumen.fluido_tasa_de_cambio = mp.tasa_de_cambio_fluido_lisado;

	volumen.tasa_de_calcificacion = mp.tasa_de_calcificacion;

	volumen.volumen_de_ruptura_relativo = 9e99;
	volumen.volumen_de_ruptura =
	volumen.total * volumen.volumen_de_ruptura_relativo;

	return;

}

bool standard_necrosis_funcion_de_arrest( Volumen& volumen, Muerte_parametros& mp ){

	if( volumen.total < volumen.volumen_de_ruptura ){

		return true;

	}

	return false;

}

void hepato30_agrandar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp){
 	volumen.target_nucleo_solido *= 1.6;
	volumen.target_citoplasma_solido *= 1.6;
    
	return;
    
}
void hepato30_achicar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp){
	volumen.target_nucleo_solido *= 1.4/1.6;
	volumen.target_citoplasma_solido *= 1.4/1.6;

	return;   
    
}

void hepato70_agrandar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp){
	volumen.target_nucleo_solido *= 2.0;
	volumen.target_citoplasma_solido *= 2.0;
    
	return;
    
}
void hepato70_achicar_funcion_de_entrada(Volumen& volumen, Muerte_parametros& mp){
	volumen.target_nucleo_solido *= 1.5/2.0;
	volumen.target_citoplasma_solido *= 1.5/2.0;
    
    
    volumen.citoplasma_tasa_de_cambio = 0.01857/60.0;
	volumen.nucleo_tasa_de_cambio = 0.01528/60.0;
	volumen.fluido_tasa_de_cambio = 0.1857/60.0;
    
	return;   
    
}

bool achicar_G0i_funcion_de_arrest(Volumen& volumen, Muerte_parametros& mp){
 	if( volumen.total <= 1.505*volumen.ideal ){
        
        //hepato_30.fases[2].actualizar_volumen = false;

		return true;

	}

	return false;   
    
}


void crear_ciclo_ki67( void )
{

	Ki67.codigo = Constantes::ciclo_Ki67;
	Ki67.nombre="ki67";
	Ki67.unidades_tiempo ="min";

	Ki67.agregar_fase( Constantes::Ki67_negativa, "ki67-" );
	Ki67.agregar_fase( Constantes::Ki67_positiva_premitotica, "ki67+ (premitotica)");
	Ki67.agregar_fase( Constantes::Ki67_positiva_postmitotica, "ki67+ (postmitotica)");

	Ki67.fases[1].division_al_final_de_la_fase = true;

	Ki67.fases[1].actualizar_volumen = true;
	Ki67.fases[2].actualizar_volumen = true;

	Ki67.agregar_link( 0 , 1, NULL ); 
	Ki67.agregar_link( 1 , 2, NULL ); 
	Ki67.agregar_link( 2 , 0, NULL ); 

	Ki67.fase_link(1,2).duracion_fija = true;
	Ki67.fase_link(2,0).duracion_fija = true;
	Ki67.fase_link(0,1).duracion_fija = false; 
    
    
//Hepatocitos
    //TQ
	Ki67.tasa_de_transicion(0,1) = 1.0/(3.0*60.0);//1/(9e9); //1.0/(19.1*60.0);///////////////////////////////////////CAMBIE
	//T1
    Ki67.tasa_de_transicion(1,2) = 1.0/(33.6*60.0);//1.0/(48.0*60.0);//1.0/(12.0*60.0);
	//T2
    Ki67.tasa_de_transicion(2,0) = 1.0/(2.5*60.0);

    
	Ki67.fases[1].funcion_de_entrada = Ki67_fase_positiva_funcion_de_entrada;

	return;
}

void crear_ciclo_ki67_cancer( void )
{

	ki67_cancer.codigo = Constantes::ciclo_Ki67_cancer;
	ki67_cancer.nombre="ki67Cancer";
	ki67_cancer.unidades_tiempo ="min";

	ki67_cancer.agregar_fase( Constantes::Ki67_negativa, "ki67-" );
	ki67_cancer.agregar_fase( Constantes::Ki67_positiva_premitotica, "ki67+ (premitotica)");
	ki67_cancer.agregar_fase( Constantes::Ki67_positiva_postmitotica, "ki67+ (postmitotica)");

	ki67_cancer.fases[1].division_al_final_de_la_fase = true;

	ki67_cancer.fases[1].actualizar_volumen = true;
	ki67_cancer.fases[2].actualizar_volumen = true;

	ki67_cancer.agregar_link( 0 , 1, NULL ); 
	ki67_cancer.agregar_link( 1 , 2, NULL ); 
	ki67_cancer.agregar_link( 2 , 0, NULL ); 

	ki67_cancer.fase_link(1,2).duracion_fija = true;
	ki67_cancer.fase_link(2,0).duracion_fija = true;
	ki67_cancer.fase_link(0,1).duracion_fija = false; 

    
//Hepatocitos
    //TQ
	ki67_cancer.tasa_de_transicion(0,1) = 1.0/(17.9*60.0);//1/(9e9); //1.0/(19.1*60.0);///////////////////////////////////////CAMBIE
	//T1
    ki67_cancer.tasa_de_transicion(1,2) = 1.0/(18.2*60.0);//1.0/(48.0*60.0);//1.0/(12.0*60.0);
	//T2
    ki67_cancer.tasa_de_transicion(2,0) = 1.0/(2.5*60.0);

    
	ki67_cancer.fases[1].funcion_de_entrada = Ki67_fase_positiva_funcion_de_entrada;

	return;
}



void crear_ciclo_vida( void )
{
	vida.codigo = Constantes::ciclo_vida;
	vida.nombre = "Vida";
	vida.unidades_tiempo = "min";

	vida.agregar_fase( Constantes::viva , "Viva" );

	vida.fases[0].actualizar_volumen = true;

	vida.fases[0].division_al_final_de_la_fase = true;

	vida.agregar_link( 0 , 0 , NULL );
    
    vida.tasa_de_transicion(0,0) = 0.0432 / 60.0; 
    
	vida.fases[0].funcion_de_entrada = fase_viva_funcion_de_entrada;

	return;
}


void crear_ciclo_necrosis(void){

	necrosis_parametros.tiempo_unidades = "min";

	necrosis_parametros.citoplasma_tasa_de_cambio = 0.0032 /60.0;
	necrosis_parametros.nucleo_tasa_de_cambio = 0.013 / 60.0;

	necrosis_parametros.tasa_de_cambio_fluido_no_lisado = 0.67/ 60.0;
	necrosis_parametros.tasa_de_cambio_fluido_lisado = 0.050 /60.0;

	necrosis_parametros.tasa_de_calcificacion = 0.0042 / 60.0;

	necrosis_parametros.volumen_de_ruptura_relativo = 2.0;

	necrosis.nombre = "Necrosis";
	necrosis.codigo = Constantes::ciclo_de_muerte_necrosis;

	necrosis.agregar_fase(Constantes::necrotica_hinchada, "Necrotica (swelling)");
	necrosis.fases[0].funcion_de_entrada = standard_necrosis_funcion_de_entrada;

	necrosis.agregar_fase(Constantes::necrotica_lisada, "Necrotica (lysed)");
	necrosis.fases[1].funcion_de_entrada = standard_lysis_funcion_de_entrada;
	necrosis.fases[1].remover_al_final_de_la_fase = true;

	necrosis.agregar_fase(Constantes::debris, "Debris");

	necrosis.fases[0].actualizar_volumen = true;//////////////////?????????????
	necrosis.fases[1].actualizar_volumen = true;

	necrosis.agregar_link(0,1,standard_necrosis_funcion_de_arrest);
	necrosis.agregar_link(1,2,NULL);

	necrosis.tasa_de_transicion(0,1) = 9e9;
	necrosis.tasa_de_transicion(1,2) = 1.0/(60.0 * 24.0 * 60.0);

	necrosis.fase_link(1,2).duracion_fija = true;

	return;

}


void crear_ciclo_hepato30( void )
{

	hepato_30.codigo = Constantes::hepato_30;
	hepato_30.nombre="Hepatocito30";
	hepato_30.unidades_tiempo ="min";

	hepato_30.agregar_fase( Constantes::G0i, "G0 inicial" );
	hepato_30.agregar_fase( Constantes::Agrandar, "Agrandar");
	hepato_30.agregar_fase( Constantes::Achicar, "Achicar");


	hepato_30.fases[1].actualizar_volumen = true;
	hepato_30.fases[2].actualizar_volumen = true;

	hepato_30.agregar_link( 0 , 1, NULL ); 
	hepato_30.agregar_link( 1 , 2, NULL ); 
	hepato_30.agregar_link( 2 , 0, achicar_G0i_funcion_de_arrest ); 


	hepato_30.fase_link(0,1).duracion_fija = true;
	hepato_30.fase_link(1,2).duracion_fija = true;
	hepato_30.fase_link(2,0).duracion_fija = true; 
        
///* //Hepatocitos 30%
 	hepato_30.tasa_de_transicion(0,1) = 1/(3.0*60.0); 
    hepato_30.tasa_de_transicion(1,2) = 1.0/(48.0*60.0);
    hepato_30.tasa_de_transicion(2,0) = 1.0/(48*60.0);
//*/

    
	hepato_30.fases[1].funcion_de_entrada = hepato30_agrandar_funcion_de_entrada;
	hepato_30.fases[2].funcion_de_entrada = hepato30_achicar_funcion_de_entrada;
    

	return;
}

void crear_ciclo_hepato70( void )
{

	hepato_70.codigo = Constantes::hepato_70;
	hepato_70.nombre="Hepatocito70";
	hepato_70.unidades_tiempo ="min";

	hepato_70.agregar_fase( Constantes::G0i, "G0 inicial" );
	hepato_70.agregar_fase( Constantes::Agrandar, "Agrandar");
	hepato_70.agregar_fase( Constantes::Achicar, "Achicar");


	hepato_70.fases[1].actualizar_volumen = true;
	hepato_70.fases[2].actualizar_volumen = true;

	hepato_70.agregar_link( 0 , 1, NULL ); 
	hepato_70.agregar_link( 1 , 2, NULL ); 
	hepato_70.agregar_link( 2 , 0, achicar_G0i_funcion_de_arrest ); 


	hepato_70.fase_link(0,1).duracion_fija = false;
	hepato_70.fase_link(1,2).duracion_fija = false;
	hepato_70.fase_link(2,0).duracion_fija = true; 
        
    
    ///* //Hepatocitos 70%
 	hepato_70.tasa_de_transicion(0,1) = 1/(3.0*60.0); 
    hepato_70.tasa_de_transicion(1,2) = 1.0/(24.0*60.0);
    hepato_70.tasa_de_transicion(2,0) = 1.0/(336.0*60.0);
//*/
    
	hepato_70.fases[1].funcion_de_entrada = hepato70_agrandar_funcion_de_entrada;
	hepato_70.fases[2].funcion_de_entrada = hepato70_achicar_funcion_de_entrada;
    

	return;
}


void crear_ciclo_apoptosis(void){
	apoptosis_parametros.tiempo_unidades = "min";

	apoptosis_parametros.citoplasma_tasa_de_cambio = 1.0 / 60.0;
	apoptosis_parametros.nucleo_tasa_de_cambio = 0.35 / 60.0;

	apoptosis_parametros.tasa_de_cambio_fluido_no_lisado = 3.0 / 60.0;
	apoptosis_parametros.tasa_de_cambio_fluido_lisado = 0.0;

	apoptosis_parametros.tasa_de_calcificacion = 0.0;

	apoptosis_parametros.volumen_de_ruptura_relativo = 2.0;

	apoptosis.nombre = "Apoptosis";
	apoptosis.codigo = Constantes::ciclo_de_muerte_apoptosis;

	apoptosis.agregar_fase( Constantes::apoptotica , "Apoptotica" );
	apoptosis.fases[0].funcion_de_entrada = standard_apoptosis_funcion_de_entrada;
	apoptosis.fases[0].remover_al_final_de_la_fase = true;

	apoptosis.agregar_fase( Constantes::debris, "Debris");

	apoptosis.agregar_link( 0, 1, NULL );
	apoptosis.tasa_de_transicion( 0, 1) = 1.0 / (8.6 * 60.0);

	apoptosis.fase_link(0,1).duracion_fija = true;

	return;

}

bool crear_ciclo_celular_estandar( void )
{
	if( ciclo_celular_estandar_inicializado ){

		return false;
	}else{

		crear_ciclo_ki67();
		crear_ciclo_vida();
        crear_ciclo_hepato30();
        crear_ciclo_hepato70();
        crear_ciclo_ki67_cancer();
		return true;
	}
}

bool crear_ciclo_de_muerte_estandar( void )
{
    //std::cout << "Entre a muerte estandar \n";
	if( ciclo_celular_de_muerte_inicializado){

		return false;
	}else{

		crear_ciclo_necrosis();
		crear_ciclo_apoptosis();
		return true;
	}

}
