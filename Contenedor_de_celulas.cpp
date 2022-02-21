#include "Contenedor_de_Celulas.h"

//std::ofstream outfile2 ("Division.dat", std::ios::app );


Contenedor_de_Celulas::Contenedor_de_Celulas(){

//	grillado;
	//celula;
	num_de_divisiones_en_este_paso=0;
    tiempo_desde_la_ultima_mecanica=0.0;
    hora_de_la_ultima_mecanica=0.0;    
//	int num_de_celulas=0;

	return;
}

void Contenedor_de_Celulas::inicializar(double x_ini, double x_fin, double y_ini, double y_fin, double z_ini, double z_fin , double dx, double dy, double dz)
{
	grillado.redimensionar(x_ini, x_fin, y_ini, y_fin, z_ini, z_fin , dx, dy, dz);
	celulas_en_voxel.resize(grillado.voxeles.size());
    for(unsigned int v=0; v<grillado.voxeles.size(); v++){
        celulas_en_voxel[v].resize(0);
    }
    


	return;
}


void Contenedor_de_Celulas::registrar_celula( Celula* celula ){

	if( !grillado.es_valida_la_posicion(celula->posicion.x,celula->posicion.y,celula->posicion.z))
	{
    std::cout << "Posicion invalida: " << celula->posicion.x << " " << celula->posicion.y << " " << celula->posicion.z << " " << celula->id << "\n ";
		celula->voxel =-1;
		agregar_celula_a_voxel(celula, celula->voxel);
		return;
	}

	celula->voxel = grillado.indice_del_voxel_mas_cercano( celula->posicion );
	//std::cout << "agregar celula a voxel \n";
	agregar_celula_a_voxel(celula, celula->voxel);

	return;

}


void Contenedor_de_Celulas::agregar_celula_a_voxel(Celula* celula, int indice_de_voxel){

	celulas_en_voxel[indice_de_voxel].push_back(celula);

}


void Contenedor_de_Celulas::agregar_potenciales_cdc(Celula* celula, Celula* otra_celula){

	if( celula->id == otra_celula->id ){
		return;
	}

//	double distancia = 0;
//	for( int i = 0 ; i < 3 ; i++ )
//	{
//		desplazamiento[i] = posicion[i] - (*otra_celula).posicion[i];
//		distancia += desplazamiento[i] * desplazamiento[i];
//	}

	celula->desplazamiento.x = celula->posicion.x - otra_celula->posicion.x;
	celula->desplazamiento.y = celula->posicion.y - otra_celula->posicion.y;
	celula->desplazamiento.z = celula->posicion.z - otra_celula->posicion.z;

    //Agrego esto por las Condiciones Periódicas de Contorno (funciona también sin CPC)
    celula->desplazamiento.x = celula->desplazamiento.x - (2*pg->rango_en_X[1])*round(celula->desplazamiento.x/(2*pg->rango_en_X[1]));
	celula->desplazamiento.y = celula->desplazamiento.y - (2*pg->rango_en_Y[1])*round(celula->desplazamiento.y/(2*pg->rango_en_Y[1]));
	celula->desplazamiento.z = celula->desplazamiento.z - (2*pg->rango_en_Z[1])*round(celula->desplazamiento.z/(2*pg->rango_en_Z[1]));

    

    
	double distancia = celula->desplazamiento.x * celula->desplazamiento.x +
	celula->desplazamiento.y * celula->desplazamiento.y + celula->desplazamiento.z * celula->desplazamiento.z;


	distancia = std::max(sqrt(distancia), 0.00001);
//
//	std::cout << "distancia: " << distancia << "\n";
//	std::cin.get();
	double R = celula->fenotipo.geometria.radio + otra_celula->fenotipo.geometria.radio;

	double temp_r;
	if( distancia > R )
	{
		temp_r=0;
	}
	else
	{
		temp_r = -distancia;
		temp_r /= R;
		temp_r += 1.0; 
		temp_r *= temp_r; 

	}

	if(celula->tipo == otra_celula->tipo){
	double repulsion_efectiva = sqrt( celula->fenotipo.mecanica.fuerza_de_repulsion_cc * otra_celula->fenotipo.mecanica.fuerza_de_repulsion_cc );
	temp_r *= repulsion_efectiva;
    }else{
	double repulsion_efectiva = sqrt( celula->fenotipo.mecanica.fuerza_de_repulsion_co * otra_celula->fenotipo.mecanica.fuerza_de_repulsion_co );
	temp_r *= repulsion_efectiva;
    }

	double distancia_de_interaccion_max = celula->fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * celula->fenotipo.geometria.radio +
		otra_celula->fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * otra_celula->fenotipo.geometria.radio;

	if(distancia < distancia_de_interaccion_max )
	{
		double temp_a = -distancia; 
		temp_a /= distancia_de_interaccion_max; 
		temp_a += 1.0; 
		temp_a *= temp_a; /

		if(celula->tipo == otra_celula->tipo){
		double adhesion_efectiva = sqrt( celula->fenotipo.mecanica.fuerza_de_adhesion_cc * otra_celula->fenotipo.mecanica.fuerza_de_adhesion_cc );
		temp_a *= adhesion_efectiva;
		}else{
		double adhesion_efectiva = sqrt( celula->fenotipo.mecanica.fuerza_de_adhesion_co * otra_celula->fenotipo.mecanica.fuerza_de_adhesion_co );
		temp_a *= adhesion_efectiva;
		}

		temp_r -= temp_a;
	}
	/////////////////////////////////////////////////////////////////
	if( fabs(temp_r) < 1e-16 )
	{ return; }
	temp_r /= distancia;

    celula->velocidad.x += temp_r * celula->desplazamiento.x;
	celula->velocidad.y += temp_r * celula->desplazamiento.y;
	celula->velocidad.z += temp_r * celula->desplazamiento.z;
    
    otra_celula->velocidad.x -= temp_r * celula->desplazamiento.x;
	otra_celula->velocidad.y -= temp_r * celula->desplazamiento.y;
	otra_celula->velocidad.z -= temp_r * celula->desplazamiento.z; 
    

    return;
}



void Contenedor_de_Celulas::actualizar_todas_las_celulas(double tiempo_total, double dt_difusion, double dt_mecanico, double dt_ciclo){


    //std::cout << "Entré a actualizar todas las células \n";
    //#pragma omp parallel for
	for (unsigned int l=0; l< todas_las_celulas.size(); l++ ){
	    //std::cout << "celula: " << todas_las_celulas[l]->id << "\n";
		todas_las_celulas[l]->simular_secrecion_y_consumo(dt_difusion);

	}
	//std::cout << "Simule secrecion y consumo \n";

    static double tolerancia_de_la_mecanica = 0.001 * dt_mecanico;
    
    tiempo_desde_la_ultima_mecanica = tiempo_total - hora_de_la_ultima_mecanica;
    if(fabs(tiempo_desde_la_ultima_mecanica - dt_mecanico) < tolerancia_de_la_mecanica){
        
        
        for(unsigned int i=0; i<grillado.voxeles.size(); i++){
            for(unsigned int j=0; j<celulas_en_voxel[i].size(); j++){
                for(unsigned int k=j+1; k<celulas_en_voxel[i].size(); k++){
                    agregar_potenciales_cdc(celulas_en_voxel[i][j], celulas_en_voxel[i][k]);
                }
                for(unsigned int n=0; n<grillado.indices_de_voxeles_conectados_tipo_moore[i].size(); n++){
                    for(unsigned int k=0; k<celulas_en_voxel[grillado.indices_de_voxeles_conectados_tipo_moore[i][n]].size(); k++){
                        agregar_potenciales_cdc(celulas_en_voxel[i][j], celulas_en_voxel[grillado.indices_de_voxeles_conectados_tipo_moore[i][n]][k]);
                    }
                }
                
            }

        }
        hora_de_la_ultima_mecanica = tiempo_total;
        
//        /*
//        #pragma omp parallel for 
        for( unsigned int a=0; a < todas_las_celulas.size(); a++ ){
            //todas_las_celulas[a]->agregar_potenciales_mb_2();
            if(todas_las_celulas[a]->tipo == 2){
                //std::cout << "Entre a linfo \n";
                todas_las_celulas[a]->actualizar_vector_de_motilidad(dt_mecanico, celulas_en_voxel[todas_las_celulas[a]->voxel]);
                todas_las_celulas[a]->avanzar_linfocito(dt_mecanico, celulas_en_voxel[todas_las_celulas[a]->voxel]);
            }
        }
//*/

        #pragma omp parallel for 
		for( unsigned int a=0; a < todas_las_celulas.size(); a++ ){
			todas_las_celulas[a]->actualizar_posicion(dt_mecanico);
            todas_las_celulas[a]->tiempo_desde_la_ultima_mecanica = tiempo_total - todas_las_celulas[a]->hora_de_la_ultima_mecanica;
            todas_las_celulas[a]->hora_de_la_ultima_mecanica = tiempo_total;
		}		        
    }


/*
	for( unsigned int a=0; a < todas_las_celulas.size(); a++ ){
//		    std::cout << "celula: " << todas_las_celulas[a]->id << "\n";

		todas_las_celulas[a]->tiempo_desde_la_ultima_mecanica = tiempo_total - todas_las_celulas[a]->hora_de_la_ultima_mecanica;

		//double tasa;
		//std::cout << "Celula: " << todas_las_celulas[a]->id << " Tiempo de la ultima mec: " << todas_las_celulas[a]->tiempo_desde_la_ultima_mecanica << std::endl;
		//std::cout<<" = "<<fabs(todas_las_celulas[a]->tiempo_desde_la_ultima_mecanica - dt_mecanico)<<"\n";
		if(fabs(todas_las_celulas[a]->tiempo_desde_la_ultima_mecanica - dt_mecanico) < tolerancia_de_la_mecanica){


		//std::cout << "Entre en la mecanica \n";

            todas_las_celulas[a]->agregar_potenciales_mb_2();

			for ( unsigned int b=0; b < celulas_en_voxel[todas_las_celulas[a]->voxel].size(); b++){

				todas_las_celulas[a]->agregar_potenciales(celulas_en_voxel[todas_las_celulas[a]->voxel][b]);
//				std::cout << "Agregue potenciales en la celulaA " << todas_las_celulas[a]->id << std::endl;
			}

			for( unsigned int c=0; c< grillado.indices_de_voxeles_conectados_tipo_moore[todas_las_celulas[a]->voxel].size(); c++){

				int voxel_vecino = grillado.indices_de_voxeles_conectados_tipo_moore[todas_las_celulas[a]->voxel][c];

				if(contiene_alguna_celula(voxel_vecino)){

					for( unsigned int d=0; d<celulas_en_voxel[voxel_vecino].size(); d++){

						todas_las_celulas[a]->agregar_potenciales(celulas_en_voxel[voxel_vecino][d]);
//						std::cout << "Agregue potenciales en la celulaB " << todas_las_celulas[a]->id << std::endl;
					}
				}
			}

			if(todas_las_celulas[a]->tipo == 2){
				//std::cout << "Entre al if de linfocitos \n";
                todas_las_celulas[a]->actualizar_vector_de_motilidad(dt_mecanico, celulas_en_voxel[todas_las_celulas[a]->voxel]);
               //std::cout << "Entre al if de linfocitos 1 \n";
                todas_las_celulas[a]->avanzar_linfocito(dt_mecanico, celulas_en_voxel[todas_las_celulas[a]->voxel]);
                //std::cout << "Entre al if de linfocitos 2 \n";
                //std::cin.get();
			}


			todas_las_celulas[a]->hora_de_la_ultima_mecanica = tiempo_total;

		}
	}
	
*/





//std::cout << "Simule la mecánica \n";


	static double tolerancia_del_fenotipo = 0.001 * dt_ciclo;
    //#pragma omp parallel for 
	for( unsigned int i=0; i < todas_las_celulas.size(); i++ ){

        todas_las_celulas[i]->tiempo_desde_el_ultimo_ciclo = tiempo_total - todas_las_celulas[i]->hora_del_ultimo_ciclo;


        if(fabs(todas_las_celulas[i]->tiempo_desde_el_ultimo_ciclo - dt_ciclo) < tolerancia_del_fenotipo){


        //std::cout << "Entre a fenotipo \n";
        	   // std::cout << "celula: " << todas_las_celulas[i]->id << "\n";
            todas_las_celulas[i]->avanzar_funciones_del_fenotipo_con_O2_y_x(tiempo_total, dt_ciclo); //oncoproteina(tiempo_total, dt_ciclo);
            //std::cout << "avance en el fenotipo \n";
            todas_las_celulas[i]->hora_del_ultimo_ciclo = tiempo_total;
		}
         //std::cout << "NO entre a fenotipo" << todas_las_celulas[i]->tiempo_desde_el_ultimo_ciclo << ", " << todas_las_celulas[i]->hora_del_ultimo_ciclo << "\n" ;



	}
//std::cout << "Simule el fenotipo \n";
/////////////////////Divido las células que están listas para dividirse
//		outfile2 << tiempo_total << " " << celulas_listas_para_dividirse.size() << "\n";/////////////-------------------------------------
	for( unsigned int i=0; i < celulas_listas_para_dividirse.size(); i++ ){
		//std::cout << "cdc 108 dividir \n";


		celulas_listas_para_dividirse[i]->dividir();

	}

	for( unsigned int i=0; i < celulas_para_registrar_en_voxeles.size(); i++ ){
		//std::cout << "cdc 115 reg en vox \n";
		registrar_celula(celulas_para_registrar_en_voxeles[i]);

	}

	for( unsigned int i=0; i < celulas_listas_para_remover.size(); i++ ){
		//std::cout << "cdc 122 remover \n";
		sacar_celula_de_voxel(celulas_listas_para_remover[i], celulas_listas_para_remover[i]->voxel);
		celulas_listas_para_remover[i]->morir(celulas_listas_para_remover[i]->indice);

	}


	num_de_divisiones_en_este_paso += celulas_listas_para_dividirse.size();
	num_de_muertes_en_este_paso += celulas_listas_para_remover.size();
	celulas_listas_para_dividirse.clear();
	celulas_listas_para_remover.clear();
	celulas_para_registrar_en_voxeles.clear();

//		std::cout << "tiempooooo " << tiempo_total << "\n";
//		for (int n=0; n<todas_las_celulas.size(); n++){
//			std::cout << "Celula id: " << todas_las_celulas[n]->id << " radio: " << todas_las_celulas[n]->fenotipo.geometria.radio << " \n";
//			std::cout << "Celula posicion: " << todas_las_celulas[n]->posicion << "\n";
//		}



	return;

}


bool Contenedor_de_Celulas::contiene_alguna_celula(int indice_de_voxel){
	return celulas_en_voxel[indice_de_voxel].size()==0?false:true;
}

void Contenedor_de_Celulas::sacar_celula_de_voxel(Celula* celula, int indice_de_voxel){

	int indice_a_borrar = 0;
	while( celulas_en_voxel[indice_de_voxel][ indice_a_borrar ] != celula )
	{
		indice_a_borrar++;
	}
	celulas_en_voxel[celula->voxel][indice_a_borrar] = celulas_en_voxel[celula->voxel][celulas_en_voxel[celula->voxel].size()-1 ];
	celulas_en_voxel[celula->voxel].pop_back();
	return;

}

void Contenedor_de_Celulas::actualizar_voxeles_de_celulas(){

	int nuevo_voxel = 0;
	for( unsigned int a=0; a<todas_las_celulas.size(); a++){

		if( !grillado.es_valida_la_posicion(todas_las_celulas[a]->posicion.x,todas_las_celulas[a]->posicion.y,todas_las_celulas[a]->posicion.z)){
			nuevo_voxel=-1;
		}else{
			nuevo_voxel= grillado.indice_del_voxel_mas_cercano( todas_las_celulas[a]->posicion );

		}

		if(todas_las_celulas[a]->voxel != nuevo_voxel){

			// #pragma omp
			sacar_celula_de_voxel(todas_las_celulas[a], todas_las_celulas[a]->voxel);
			agregar_celula_a_voxel(todas_las_celulas[a], nuevo_voxel);

			todas_las_celulas[a]->voxel = nuevo_voxel;

		}


	}
	return;

}
