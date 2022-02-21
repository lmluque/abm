#include "Celula.h"

//std::ofstream outfile2 ("Division.dat", std::ios::app );

Celula::Celula(){

	nombre = "Sin Nombre";
    tipo = 0;
	id = 9999;
    indice = 9999;
    voxel = 9999;
    madre = 0;
    tiempo_desde_el_ultimo_ciclo = 0.0;
    tiempo_desde_la_ultima_mecanica = 0.0;
    hora_del_ultimo_ciclo = 0.0;
    hora_de_la_ultima_mecanica = 0.0;
    adherida = false;
//    fenotipo;
	posicion.x=-0.0;
	posicion.y=0.0;
	posicion.z=0.0;
//	velocidad;
//    velocidad_anterior;
//    desplazamiento;
    //Punteros a funciones
    celula_adherida = NULL;
    probabilidad=pg->probabilidad;//.0;
    probabilidad_aleat=0.0;
//    divisiones_permitidas=round(rng->RandomNumber( 1.0, 2.0 ));
    //divisiones_permitidas= 1-round(rng->RandomNumber() - 0.201); //Promedio de división 0.7
    divisiones_permitidas= round(rng->RandomNumber()); //Promedio de división 0.5

    divisiones=0;
    

    /*Microambiente y oxígeno*/
    es_activa=true;
    microambiente = NULL;
	if( get_microambiente_default() != NULL )
	{ microambiente = get_microambiente_default(); }
	registrar_microambiente(microambiente);


	return;
}


void Celula::actualizar_volumen( Fenotipo& fenotipo, double dt)
{

	fenotipo.volumen.fluido += dt* fenotipo.volumen.fluido_tasa_de_cambio *
	(fenotipo.volumen.target_fraccion_fluido * fenotipo.volumen.total - fenotipo.volumen.fluido);


	//Si el volumen del fluido es negativo, set a 0
	if (fenotipo.volumen.fluido < 0.0){
		fenotipo.volumen.fluido = 0.0;
	}

	fenotipo.volumen.nuclear_fluido = (fenotipo.volumen.nuclear / fenotipo.volumen.total) *
	(fenotipo.volumen.fluido);
	fenotipo.volumen.citoplasmatico_fluido = fenotipo.volumen.fluido - fenotipo.volumen.nuclear_fluido;


	fenotipo.volumen.nuclear_solido += dt* fenotipo.volumen.nucleo_tasa_de_cambio *
	(fenotipo.volumen.target_nucleo_solido - fenotipo.volumen.nuclear_solido);

	if(fenotipo.volumen.nuclear_solido <0.0){
		fenotipo.volumen.nuclear_solido= 0.0;
	}


	fenotipo.volumen.target_citoplasma_solido = fenotipo.volumen.target_relacion_citoplasma_nucleo *
	fenotipo.volumen.target_nucleo_solido;

	fenotipo.volumen.citoplasmatico_solido += dt* fenotipo.volumen.citoplasma_tasa_de_cambio *
	(fenotipo.volumen.target_citoplasma_solido - fenotipo.volumen.citoplasmatico_solido);

	if(fenotipo.volumen.citoplasmatico_solido <0.0){
		fenotipo.volumen.citoplasmatico_solido= 0.0;
	}

	fenotipo.volumen.solido = fenotipo.volumen.nuclear_solido + fenotipo.volumen.citoplasmatico_solido;

	fenotipo.volumen.nuclear = fenotipo.volumen.nuclear_solido + fenotipo.volumen.nuclear_fluido;
	fenotipo.volumen.citoplasmatico = fenotipo.volumen.citoplasmatico_solido + fenotipo.volumen.citoplasmatico_fluido;

	fenotipo.volumen.fraccion_calcificada = dt* fenotipo.volumen.tasa_de_calcificacion *
	(1- fenotipo.volumen.fraccion_calcificada);

	fenotipo.volumen.total = fenotipo.volumen.citoplasmatico + fenotipo.volumen.nuclear;

	fenotipo.volumen.fraccion_de_fluido = fenotipo.volumen.fluido / (1e-16 + fenotipo.volumen.total);


////	fenotipo.volumen.total += dt * fenotipo.volumen.tasa_de_cambio*
////		( fenotipo.volumen.target - fenotipo.volumen.total )* fenotipo.ciclo.tasa_de_transicion();
////
   	fenotipo.geometria.actualizar(fenotipo.volumen);

	return;
}


void Celula::actualizar_parametros_de_celula_y_muerte_con_o2(Fenotipo& fenotipo, double dt){

	//std::cout <<"Actualizar parametros O2 \n";
	if(fenotipo.muerte.muerta == true){
		return;
	}
//    std::cout << "O2 flag 1 \n";
	static bool indices_iniciados = false;
	static int indice_fase_inicial;
	static int indice_fase_final;
	static int indice_necrosis;

	static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
	//std::cout <<"No pase el microambiente \n";
//std::cout << "O2 flag 2 \n";
	if(indices_iniciados == false){

		if(fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_Ki67){
			//std::cout <<"Entre al if del ciclo \n";
			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_negativa);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			//std::cout << "INDICE NECROSIS = " << indice_necrosis << "\n";
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_positiva_premitotica);
			indices_iniciados = true;

		}

		if( fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_vida ){

			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::viva);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::viva);
			indices_iniciados = true;
		}
	}
	//std::cout <<"Sali del if del ciclo \n";
//std::cout << "O2 flag 3 \n";
	double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
//	int n = fenotipo.ciclo.indice_de_la_fase_actual;
	//std::cout <<"pO2 y n \n";

	double multiplicador = 1.0;
	if(pO2 < parametros.o2_saturacion_para_la_proliferacion){
	//	std::cout <<"multiplicador 1 \n";
		multiplicador = (pO2 - parametros.o2_limite_de_proliferacion) /
		(parametros.o2_saturacion_para_la_proliferacion - parametros.o2_limite_de_proliferacion);
	}

	if(pO2 < parametros.o2_limite_de_proliferacion){
	//	std::cout <<"multiplicador 2 \n";
		multiplicador= 0.0;
	}
//std::cout << "O2 flag 4 \n";


	fenotipo.ciclo.tasas_de_transicion[indice_fase_inicial][indice_fase_final] = multiplicador *
	fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_fase_inicial, indice_fase_final);
	//std::cout <<"actualizar las tasas \n";
//std::cout << "O2 flag 5 \n";


	multiplicador = 0.0;
	if(pO2 < parametros.o2_necrosis_limite){
	//	std::cout <<"multiplicador necro 1 \n";
		multiplicador = (parametros.o2_necrosis_limite - pO2) /
		(parametros.o2_necrosis_limite - parametros.o2_necrosis_max);
	}
//std::cout << "O2 flag 5.1 \n";
	if(pO2 < parametros.o2_necrosis_max){
	//	std::cout <<"multiplicador necro 2 \n";
		multiplicador = 1.0;
	}
	//std::cout << "O2 flag 5.2 \n";
	//std::cout <<"No entre a los ifs necro";
	//std::cout << "TASAS? " << fenotipo.muerte.tasas[indice_necrosis] <<"\n";
	fenotipo.muerte.tasas[indice_necrosis] = multiplicador * parametros.tasa_necrosis_max;
	//std::cout << "O2 flag 5.3 \n";
	//std::cout <<"tasas necro \n";
	/*???????Hace falta????????
	-Falta la funcion remover celula
	-Chequear de agregar el extern del vector remover
	*/
//std::cout << "O2 flag 6 \n";
}


void Celula::actualizar_parametros_de_celula_y_muerte_con_o2_y_oncoproteina(Fenotipo& fenotipo, double dt){

	//std::cout <<"Actualizar parametros O2 \n";
	if(fenotipo.muerte.muerta == true || tipo != 0){
		return;
	}
//    std::cout << "O2 flag 1 \n";
	static bool indices_iniciados = false;
	static int indice_fase_inicial;
	static int indice_fase_final;
	static int indice_necrosis;

	static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
	//std::cout <<"No pase el microambiente \n";
//std::cout << "O2 flag 2 \n";
	if(indices_iniciados == false){

		if(fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_Ki67){
			//std::cout <<"Entre al if del ciclo \n";
			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_negativa);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			//std::cout << "INDICE NECROSIS = " << indice_necrosis << "\n";
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_positiva_premitotica);
			indices_iniciados = true;

		}

		if( fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_vida ){

			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::viva);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::viva);
			indices_iniciados = true;
		}
	}
	//std::cout <<"Sali del if del ciclo \n";
//std::cout << "O2 flag 3 \n";
	double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
//	int n = fenotipo.ciclo.indice_de_la_fase_actual;
	//std::cout <<"pO2 y n \n";

	double multiplicador = 1.0;
	if(pO2 < parametros.o2_saturacion_para_la_proliferacion){
	//	std::cout <<"multiplicador 1 \n";
		multiplicador = (pO2 - parametros.o2_limite_de_proliferacion) /
		(parametros.o2_saturacion_para_la_proliferacion - parametros.o2_limite_de_proliferacion);
	}

	if(pO2 < parametros.o2_limite_de_proliferacion){
	//	std::cout <<"multiplicador 2 \n";
		multiplicador= 0.0;
	}
//std::cout << "O2 flag 4 \n";

	fenotipo.ciclo.tasas_de_transicion[indice_fase_inicial][indice_fase_final] = multiplicador *
	fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_fase_inicial, indice_fase_final);
	//std::cout <<"actualizar las tasas \n";
//std::cout << "O2 flag 5 \n";
	// Update necrosis rate

	multiplicador = 0.0;
	if(pO2 < parametros.o2_necrosis_limite){
	//	std::cout <<"multiplicador necro 1 \n";
		multiplicador = (parametros.o2_necrosis_limite - pO2) /
		(parametros.o2_necrosis_limite - parametros.o2_necrosis_max);
	}
//std::cout << "O2 flag 5.1 \n";
	if(pO2 < parametros.o2_necrosis_max){
	//	std::cout <<"multiplicador necro 2 \n";
		multiplicador = 1.0;
	}
	//std::cout << "O2 flag 5.2 \n";
	//std::cout <<"No entre a los ifs necro";
	//std::cout << "TASAS? " << fenotipo.muerte.tasas[indice_necrosis] <<"\n";
	fenotipo.muerte.tasas[indice_necrosis] = multiplicador * parametros.tasa_necrosis_max;
	//std::cout << "O2 flag 5.3 \n";
	//std::cout <<"tasas necro \n";
	/*???????Hace falta????????
	-Falta la funcion remover celula
	-Chequear de agregar el extern del vector remover
	*/
//std::cout << "O2 flag 6 \n";

/////////ACÄ ENTRAA EN JUEGO LA ONCOPROTEINA /////////////////

    fenotipo.ciclo.tasas_de_transicion[indice_fase_inicial][indice_fase_final] *= fenotipo.secrecion.oncoproteina;

}

void Celula::actualizar_parametros_de_celula_y_muerte_con_o2_y_x(Fenotipo& fenotipo, double dt){

	//std::cout <<"Actualizar parametros O2 \n";
	if(fenotipo.muerte.muerta == true || tipo != 0){
		return;
	}
//    std::cout << "O2 flag 1 \n";
	static bool indices_iniciados = false;
	static int indice_fase_inicial;
	static int indice_fase_final;
	static int indice_necrosis;

	static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
    static int indice_de_x = microambiente->encontrar_indice_de_densidad("sustancia x");
	//std::cout <<"No pase el microambiente \n";
//std::cout << "O2 flag 2 \n";
	if(indices_iniciados == false){

		if(fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::hepato_30 || fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::hepato_70){
			//std::cout <<"Entre al if del ciclo \n";
			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::G0i);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			//std::cout << "INDICE NECROSIS = " << indice_necrosis << "\n";
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Agrandar);
			indices_iniciados = true;

		}
		
		if(fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_Ki67 || fenotipo.ciclo.pCiclo_Modelo->codigo == Constantes::ciclo_Ki67_cancer){
			//std::cout <<"Entre al if del ciclo \n";
			indice_fase_inicial = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_negativa);
			indice_necrosis = fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte(Constantes::ciclo_de_muerte_necrosis);
			//std::cout << "INDICE NECROSIS = " << indice_necrosis << "\n";
			indice_fase_final = fenotipo.ciclo.pCiclo_Modelo->encontrar_indice_de_la_fase(Constantes::Ki67_positiva_premitotica);
			indices_iniciados = true;

		}

	}
	//std::cout <<"Sali del if del ciclo \n";
//std::cout << "O2 flag 3 \n";
	double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
    double pX = (vector_de_densidades_mas_cercano())[indice_de_x];
//	int n = fenotipo.ciclo.indice_de_la_fase_actual;
	//std::cout <<"pO2 y n \n";

	double multiplicador = 1.0;
	if(pO2 < parametros.o2_saturacion_para_la_proliferacion){
	//	std::cout <<"multiplicador 1 \n";
		multiplicador = (pO2 - parametros.o2_limite_de_proliferacion) /
		(parametros.o2_saturacion_para_la_proliferacion - parametros.o2_limite_de_proliferacion);
	}

	if(pO2 < parametros.o2_limite_de_proliferacion){
	//	std::cout <<"multiplicador 2 \n";
		multiplicador= 0.0;
	}
//std::cout << "O2 flag 4 \n";

    double multiplicador2 =1.0 ;
    if(pg->hepatectomia==false){//(pX < parametros.x_corte30){
        multiplicador2=0.0;
        fenotipo.ciclo.tiempo_acumulado_en_la_fase = 0.0;
        
    }else{
        
        if(tipo==1){
            
            if(pX > parametros.X_limite_de_proliferacion){
                
                multiplicador2 = 0.0;
                
            }else if(pX > parametros.X_saturacion_para_la_proliferacion){
                
                multiplicador2 = 1-((pX - parametros.X_limite_de_proliferacion) /
                (parametros.X_saturacion_para_la_proliferacion - parametros.X_limite_de_proliferacion));
            }
            
        }
        
        
        if(tipo==0){
            
            if(pX < parametros.X_limite_de_proliferacion){
                
                multiplicador2 = 0.0;
                
            }
            
        }
        
        
    }


	//fenotipo.ciclo.tasas_de_transicion[indice_fase_inicial][indice_fase_final] = multiplicador2 * multiplicador *
	//fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_fase_inicial, indice_fase_final);
	
	fenotipo.ciclo.actualizar_mis_tasas_de_transicion(indice_fase_inicial, indice_fase_final) = multiplicador2 * multiplicador * 
	fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_fase_inicial, indice_fase_final);
	
//     if(tipo == 0){
//         std::cout << multiplicador << " " << multiplicador2 << " " << " " << fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_fase_inicial, indice_fase_final) << " " << fenotipo.ciclo.actualizar_mis_tasas_de_transicion(indice_fase_inicial, indice_fase_final) << "\n";
//         std::cin.get();
//     }
	//std::cout <<"actualizar las tasas \n";
//std::cout << "O2 flag 5 \n";

	multiplicador = 0.0;
	if(pO2 < parametros.o2_necrosis_limite){
	//	std::cout <<"multiplicador necro 1 \n";
		multiplicador = (parametros.o2_necrosis_limite - pO2) /
		(parametros.o2_necrosis_limite - parametros.o2_necrosis_max);
	}
//std::cout << "O2 flag 5.1 \n";
	if(pO2 < parametros.o2_necrosis_max){
	//	std::cout <<"multiplicador necro 2 \n";
		multiplicador = 1.0;
	}
	//std::cout << "O2 flag 5.2 \n";
	//std::cout <<"No entre a los ifs necro";
	//std::cout << "TASAS? " << fenotipo.muerte.tasas[indice_necrosis] <<"\n";
	fenotipo.muerte.tasas[indice_necrosis] = multiplicador * parametros.tasa_necrosis_max;
	//std::cout << "O2 flag 5.3 \n";
	//std::cout <<"tasas necro \n";
	/*???????Hace falta????????
	-Falta la funcion remover celula
	-Chequear de agregar el extern del vector remover
	*/
//std::cout << "O2 flag 6 \n";


}


void Celula::avanzar_funciones_del_fenotipo(double hora_global, double dt_ciclo){


//	tiempo_desde_el_ultimo_ciclo = hora_global - hora_del_ultimo_ciclo;

//	static double tolerancia_del_fenotipo = 0.001 * dt_ciclo;
//	double tasa;

//	if(fabs(tiempo_desde_el_ultimo_ciclo - dt_ciclo) < tolerancia_del_fenotipo){
	//std::cout << "Entre en el fenotipo \n";

		actualizar_parametros_de_celula_y_muerte_con_o2_y_x(fenotipo, dt_ciclo);

	//	std::cout << "Pase el pO2 \n";
		if(fenotipo.muerte.chequear_muerte(dt_ciclo) == true){
	//	std::cout << "If de muerte \n";
			fenotipo.ciclo.sync_con_ciclo_modelo(fenotipo.muerte.ciclo_actual());

			fenotipo.secrecion.set_todas_las_secreciones_a_cero();
			fenotipo.secrecion.multiplicar_los_consumos_por_un_factor(0.10);

			if(fenotipo.ciclo.fase_actual().funcion_de_entrada){

				fenotipo.ciclo.fase_actual().funcion_de_entrada(fenotipo.volumen, fenotipo.muerte.parametros_actuales());

			}

		}//std::cout << "TIEMPO DESDE EL ULTIMO CICLO = " << tiempo_desde_el_ultimo_ciclo << "\n";
		//std::cin.get();
	//	std::cout << "fuera del If de muerte \n";

		fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );
	//	std::cout << "Avance en el ciclo \n";

		if(fenotipo.ciclo.actualizar_volumen()){

			actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
			fenotipo.volumen.cambio_el_volumen=true;
		}

		if(fenotipo.ciclo.flagged_para_dividirse){

            if(fenotipo.muerte.muerta){
                    std::cout<< "Se divide celula muerta id: " << id << "\n";
                    std::cin.get();
            }

			celulas_listas_para_dividirse.push_back(this);
			fenotipo.ciclo.flagged_para_dividirse=false;
			//std::cout << "celulas para dividir: " << celulas_listas_para_dividirse.size() << std::endl;
			//outfile2 << hora_global << " " << celulas_listas_para_dividirse.size() << "\n";/////////////-------------------------------------
		}

		if(fenotipo.ciclo.flagged_para_remover){
			celulas_listas_para_remover.push_back(this);
			fenotipo.ciclo.flagged_para_remover=false;
		}

		//hora_del_ultimo_ciclo = hora_global;


	return;
}

void Celula::avanzar_funciones_del_fenotipo_con_O2(double hora_global, double dt_ciclo){
  //  std::cout << "avanzar func del feno \n";


        static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
        double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
   // std::cout << "flag 1 \n";
		actualizar_parametros_de_celula_y_muerte_con_o2(fenotipo, dt_ciclo);
//   std::cout << "flag 2 \n";
		if(fenotipo.muerte.chequear_muerte(dt_ciclo) == true){
			fenotipo.ciclo.sync_con_ciclo_modelo(fenotipo.muerte.ciclo_actual());
//   std::cout << "flag 3 \n";
			fenotipo.secrecion.set_todas_las_secreciones_a_cero();
			fenotipo.secrecion.multiplicar_los_consumos_por_un_factor(0.10);

			if(fenotipo.ciclo.fase_actual().funcion_de_entrada){

				fenotipo.ciclo.fase_actual().funcion_de_entrada(fenotipo.volumen, fenotipo.muerte.parametros_actuales());

			}

		}
//   std::cout << "flag 4 \n";
        if(!fenotipo.muerte.muerta && pO2 > parametros.o2_limite_de_proliferacion){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_dividirse){

                celulas_listas_para_dividirse.push_back(this);
                fenotipo.ciclo.flagged_para_dividirse=false;
//                std::cout << "celulas para dividir: " << celulas_listas_para_dividirse.size() << std::endl;

            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

  // std::cout << "flag 5 \n";
        }else if(fenotipo.muerte.muerta){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

        }
  // std::cout << "flag 6 \n";
	return;
}

void Celula::avanzar_funciones_del_fenotipo_con_O2_y_oncoproteina(double hora_global, double dt_ciclo){
    //std::cout << hora_global << "avanzar func del feno \n";
        if(tipo!=2 && adherida==true){
            return;
        }


        static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
        double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
   //std::cout << "flagf 1 \n";
		actualizar_parametros_de_celula_y_muerte_con_o2_y_oncoproteina(fenotipo, dt_ciclo);
   //std::cout << "flagf 2 \n";
		if(fenotipo.muerte.chequear_muerte(dt_ciclo) == true){
			fenotipo.ciclo.sync_con_ciclo_modelo(fenotipo.muerte.ciclo_actual());
            fenotipo.ciclo.indice_de_la_fase_actual = 0;
            fenotipo.ciclo.tiempo_acumulado_en_la_fase = 0.0;
   //std::cout << "flagf 3 \n";
			fenotipo.secrecion.set_todas_las_secreciones_a_cero();
			fenotipo.secrecion.multiplicar_los_consumos_por_un_factor(0.10);
			if(tipo==2){
                es_movil(false);
                if(adherida==true){
                    adherida=false;
                    celula_adherida->adherida=false;
                    celula_adherida=NULL;
                }

			}
//std::cout << "flagf4 \n";
			if(fenotipo.ciclo.fase_actual().funcion_de_entrada){

				fenotipo.ciclo.fase_actual().funcion_de_entrada(fenotipo.volumen, fenotipo.muerte.parametros_actuales());

			}


		}
		
		//std::cout << "flagf5 \n";
		if(tipo==2 && fenotipo.muerte.muerta == false){
            fenotipo.muerte.tasas[0] = 1/abs(((1/fenotipo.muerte.tasas[0]) - (dt_ciclo*dt_ciclo)));
            //std::cout << pg->tiempo_total << " " << id << " " << fenotipo.muerte.tasas[0] << "\n";
		}
		//std::cout << "flagf6 \n";

//   std::cout << "flag 4 \n";
        if(fenotipo.muerte.muerta == false && pO2 > parametros.o2_limite_de_proliferacion && probabilidad_aleat < probabilidad && divisiones < divisiones_permitidas){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_dividirse){

                celulas_listas_para_dividirse.push_back(this);
                fenotipo.ciclo.flagged_para_dividirse=false;
//                std::cout << "celulas para dividir: " << celulas_listas_para_dividirse.size() << std::endl;

            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

   //std::cout << "flagf 7 \n";
        }else if(fenotipo.muerte.muerta == true){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

        }
  // std::cout << "flag 6 \n";
	return;
}

void Celula::avanzar_funciones_del_fenotipo_con_O2_y_x(double hora_global, double dt_ciclo){
    //std::cout << hora_global << "avanzar func del feno \n";
        if(tipo!=2 && adherida==true){
            return;
        }


        static int indice_del_oxigeno = microambiente->encontrar_indice_de_densidad("oxigeno");
        double pO2 = (vector_de_densidades_mas_cercano())[indice_del_oxigeno];
   //std::cout << "flagf 1 \n";

        actualizar_parametros_de_celula_y_muerte_con_o2_y_x(fenotipo, dt_ciclo);

   //std::cout << "flagf 2 \n";
		if(fenotipo.muerte.chequear_muerte(dt_ciclo) == true){
			fenotipo.ciclo.sync_con_ciclo_modelo(fenotipo.muerte.ciclo_actual());
            fenotipo.ciclo.indice_de_la_fase_actual = 0;
            fenotipo.ciclo.tiempo_acumulado_en_la_fase = 0.0;
   //std::cout << "flagf 3 \n";
			fenotipo.secrecion.set_todas_las_secreciones_a_cero();
			fenotipo.secrecion.multiplicar_los_consumos_por_un_factor(0.10);
			if(tipo==2){
                es_movil(false);
                if(adherida==true){
                    adherida=false;
                    celula_adherida->adherida=false;
                    celula_adherida=NULL;
                }

			}
//std::cout << "flagf4 \n";
			if(fenotipo.ciclo.fase_actual().funcion_de_entrada){

				fenotipo.ciclo.fase_actual().funcion_de_entrada(fenotipo.volumen, fenotipo.muerte.parametros_actuales());

			}


		}
		
		//std::cout << "flagf5 \n";
		if(tipo==2 && fenotipo.muerte.muerta == false){
            fenotipo.muerte.tasas[0] = 1/abs(((1/fenotipo.muerte.tasas[0]) - (dt_ciclo*dt_ciclo)));
            //std::cout << pg->tiempo_total << " " << id << " " << fenotipo.muerte.tasas[0] << "\n";
		}
		//std::cout << "flagf6 \n";

//   std::cout << "flag 4 \n";
        if(fenotipo.muerte.muerta == false && pO2 > parametros.o2_limite_de_proliferacion && divisiones < divisiones_permitidas){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_dividirse){

                celulas_listas_para_dividirse.push_back(this);
                fenotipo.ciclo.flagged_para_dividirse=false;
//                std::cout << "celulas para dividir: " << celulas_listas_para_dividirse.size() << std::endl;

            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

   //std::cout << "flagf 7 \n";
        }else if(fenotipo.muerte.muerta == true){

            fenotipo.ciclo.avanzar_en_el_ciclo( fenotipo.volumen, tiempo_desde_el_ultimo_ciclo, fenotipo.ciclo.tasas_de_transicion, fenotipo.muerte.parametros_actuales() );

            if(fenotipo.ciclo.actualizar_volumen()){

                actualizar_volumen(fenotipo, tiempo_desde_el_ultimo_ciclo);
                fenotipo.volumen.cambio_el_volumen=true;
            }

            if(fenotipo.ciclo.flagged_para_remover){
                celulas_listas_para_remover.push_back(this);
                fenotipo.ciclo.flagged_para_remover=false;
            }

        }
  // std::cout << "flag 6 \n";
	return;
}


Celula* crear_celula(void){ 

    Celula* pNuevo;
	pNuevo = new Celula;
	todas_las_celulas.push_back(pNuevo);
	celulas_para_registrar_en_voxeles.push_back(pNuevo);
	pNuevo->indice=todas_las_celulas.size()-1;
	pg->numero_id += 1;
	pNuevo->id=pg->numero_id;
    pNuevo->probabilidad_aleat=rng->RandomNumber();
    pNuevo->divisiones_permitidas= round(rng->RandomNumber()); //Promedio de división 0.5 //1-round(rng->RandomNumber() - 0.201);
	//pNuevo->id=todas_las_celulas.size();


	return pNuevo;
}

Linfocito* crear_linfocito(void){ 

    Linfocito* pNuevo;
	pNuevo = new Linfocito;
	todas_las_celulas.push_back(pNuevo);
	celulas_para_registrar_en_voxeles.push_back(pNuevo);
	pNuevo->indice=todas_las_celulas.size()-1;
	pg->numero_id += 1;
	pNuevo->id=pg->numero_id;
//	pNuevo->id=todas_las_celulas.size();

	return pNuevo;
}

bool Celula::set_posicion(double x, double y, double z){

	if (pg->condiciones_de_periodicidad) {
		if (x < pg->rango_en_X[0] && pg->condiciones_de_periodicidad_x )  {
			x = pg->rango_en_X[1] - (pg->rango_en_X[0] - x);
		} else if (x > pg->rango_en_X[1] && pg->condiciones_de_periodicidad_x ) {
			x = pg->rango_en_X[0] + (x - pg->rango_en_X[1]);
		}
		if (y < pg->rango_en_Y[0] && pg->condiciones_de_periodicidad_y ) {
			y = pg->rango_en_Y[1] - (pg->rango_en_Y[0] - y);
		} else if (y > pg->rango_en_Y[1] && pg->condiciones_de_periodicidad_y ) {
			y = pg->rango_en_Y[0] + (y - pg->rango_en_Y[1]);
		}
		if (z < pg->rango_en_Z[0] && pg->condiciones_de_periodicidad_z ) {
			z = pg->rango_en_Z[1] - (pg->rango_en_Z[0] - z);
		} else if (z > pg->rango_en_Z[1] && pg->condiciones_de_periodicidad_z ) {
			z = pg->rango_en_Z[0] + (z - pg->rango_en_Z[1]);
		}
	}
	posicion.x=x;
	posicion.y=y;
	posicion.z=z;

	return true;
}

bool Celula::set_posicion(Vector posicion){

    return set_posicion(posicion.x, posicion.y, posicion.z);
}


Celula* Celula::dividir(){

	Celula* hija = crear_celula();
	hija->tipo = tipo;
	hija->madre = id;
	hija->nombre = nombre;
	hija->hora_del_ultimo_ciclo = hora_del_ultimo_ciclo;
	hija->hora_de_la_ultima_mecanica = hora_de_la_ultima_mecanica;
	hija->parametros = parametros;
    if(tipo==0){
        hija->divisiones_permitidas = divisiones_permitidas;
    }


	double angulo_temporal = 6.28318530717959*rng->RandomNumber();
	double phi_temporal = 3.1415926535897932384626433832795*rng->RandomNumber();

	double radio = fenotipo.geometria.radio;
	Vector vector_random (0.0, 0.0, 0.0);

    if(pg->crecer_al_costado){
    	if(rng->RandomNumber()<0.05){
            vector_random.x= cos( angulo_temporal ) * sin( phi_temporal );
            vector_random.y= sin( angulo_temporal ) * sin( phi_temporal );
            vector_random.z= cos( phi_temporal );
            }else{
                vector_random.x= cos( angulo_temporal );
                vector_random.y= sin( angulo_temporal );
                vector_random.z= 0;
                }
        }else{
            vector_random.x= cos( angulo_temporal ) * sin( phi_temporal );
            vector_random.y= sin( angulo_temporal ) * sin( phi_temporal );
            vector_random.z= cos( phi_temporal );
        }


	//Le asigno una posición a la hija
	hija->set_posicion(posicion.x + 0.206299474 * radio*vector_random.x, // estaba multiplicado por 0.5 en vez de 0.206299474
						 posicion.y + 0.206299474 * radio*vector_random.y,
						 posicion.z + 0.206299474 * radio*vector_random.z);
	hija->actualizar_voxel_del_microambiente();

	//Modifico la posición de la madre
	set_posicion(posicion.x - 0.206299474*radio*vector_random.x,
		posicion.y - 0.206299474*radio*vector_random.y,
		posicion.z - 0.206299474*radio*vector_random.z);
	actualizar_voxel_del_microambiente();

	fenotipo.volumen.dividir();
	fenotipo.geometria.actualizar(fenotipo.volumen);
	hija->fenotipo = fenotipo;
    
    if(tipo==1){
        
        divisiones+=1;
    
        //No le estaba asignando el ciclo según el número aleatorio. Heredaba el ciclo de la madre
        if(hija->divisiones_permitidas==0){
            hija->fenotipo.ciclo.sync_con_ciclo_modelo(hepato_70);
            hija->fenotipo.ciclo.indice_de_la_fase_actual = 0;
            hija->fenotipo.ciclo.tiempo_acumulado_en_la_fase = 0.0;
            hija->divisiones_permitidas=1; //En avanzar fenotipo necesito que sea 1, pero no afecta nada ya que el ciclo no le permite proliferar.
        }else{
            hija->fenotipo.ciclo.sync_con_ciclo_modelo(Ki67);
        }
    
    }

//	//Poner sólo como ejemplo. Después sacar. /////////
//    hija->fenotipo.secrecion.oncoproteina = rng->NormalRandom( pg->imm_mean, pg->imm_sd );
//    if( hija->fenotipo.secrecion.oncoproteina < 0.0 ){
//    hija->fenotipo.secrecion.oncoproteina = 0.0; }
//    ///////////////////////////////////////////////////

	return hija;
}



void Celula::morir(int indice){

	delete todas_las_celulas[indice];

	todas_las_celulas[ todas_las_celulas.size()-1 ]->indice=indice;
	todas_las_celulas[indice] = todas_las_celulas[ todas_las_celulas.size()-1 ];
	todas_las_celulas.pop_back();
	return;


}


void Celula::agregar_potenciales(Celula* otra_celula){

	if( this->id == otra_celula->id ){
		return;
	}

//	double distancia = 0;
//	for( int i = 0 ; i < 3 ; i++ )
//	{
//		desplazamiento[i] = posicion[i] - (*otra_celula).posicion[i];
//		distancia += desplazamiento[i] * desplazamiento[i];
//	}

	desplazamiento.x = posicion.x - otra_celula->posicion.x;
	desplazamiento.y = posicion.y - otra_celula->posicion.y;
	desplazamiento.z = posicion.z - otra_celula->posicion.z;

    //Agrego esto por las Condiciones Periódicas de Contorno (funciona también sin CPC)
    desplazamiento.x = desplazamiento.x - (2*pg->rango_en_X[1])*round(desplazamiento.x/(2*pg->rango_en_X[1]));
	desplazamiento.y = desplazamiento.y - (2*pg->rango_en_Y[1])*round(desplazamiento.y/(2*pg->rango_en_Y[1]));
	desplazamiento.z = desplazamiento.z - (2*pg->rango_en_Z[1])*round(desplazamiento.z/(2*pg->rango_en_Z[1]));




	double distancia = desplazamiento.x * desplazamiento.x +
	desplazamiento.y * desplazamiento.y + desplazamiento.z * desplazamiento.z;


	distancia = std::max(sqrt(distancia), 0.00001);
//
//	std::cout << "distancia: " << distancia << "\n";
//	std::cin.get();
	double R = fenotipo.geometria.radio + otra_celula->fenotipo.geometria.radio;

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

	if(this->tipo == otra_celula->tipo){
	double repulsion_efectiva = sqrt( fenotipo.mecanica.fuerza_de_repulsion_cc * otra_celula->fenotipo.mecanica.fuerza_de_repulsion_cc );
	temp_r *= repulsion_efectiva;
    }else{
	double repulsion_efectiva = sqrt( fenotipo.mecanica.fuerza_de_repulsion_co * otra_celula->fenotipo.mecanica.fuerza_de_repulsion_co );
	temp_r *= repulsion_efectiva;
    }

	double distancia_de_interaccion_max = fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * fenotipo.geometria.radio +
		otra_celula->fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * otra_celula->fenotipo.geometria.radio;

	if(distancia < distancia_de_interaccion_max )
	{
		double temp_a = -distancia;
		temp_a /= distancia_de_interaccion_max;
		temp_a += 1.0;
		temp_a *= temp_a; 

		if(this->tipo == otra_celula->tipo){
		double adhesion_efectiva = sqrt( fenotipo.mecanica.fuerza_de_adhesion_cc * otra_celula->fenotipo.mecanica.fuerza_de_adhesion_cc );
		temp_a *= adhesion_efectiva;
		}else{
		double adhesion_efectiva = sqrt( fenotipo.mecanica.fuerza_de_adhesion_co * otra_celula->fenotipo.mecanica.fuerza_de_adhesion_co );
		temp_a *= adhesion_efectiva;
		}

		temp_r -= temp_a;
	}
	/////////////////////////////////////////////////////////////////
	if( fabs(temp_r) < 1e-16 )
	{ return; }
	temp_r /= distancia;
	
    velocidad.x += temp_r * desplazamiento.x;
	velocidad.y += temp_r * desplazamiento.y;
	velocidad.z += temp_r * desplazamiento.z;
    
    //Le agrego el potencial a la otra célula también
    otra_celula->velocidad.x -= temp_r * desplazamiento.x;
	otra_celula->velocidad.y -= temp_r * desplazamiento.y;
	otra_celula->velocidad.z -= temp_r * desplazamiento.z; 

	return;
}

void Celula::agregar_potenciales_mb(){

    if( pg->interactuar_con_mb==false ){
		return;
	}


	desplazamiento.x = posicion.x - posicion.x;
	desplazamiento.y = posicion.y - posicion.y;
	desplazamiento.z = posicion.z - pg->rango_en_Z[0];

	double distancia = desplazamiento.x * desplazamiento.x +
	desplazamiento.y * desplazamiento.y + desplazamiento.z * desplazamiento.z;


	distancia = std::max(sqrt(distancia), 0.00001);


	double distancia_de_interaccion_max = fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * fenotipo.geometria.radio;

	double temp_a=0;
	if(distancia < distancia_de_interaccion_max)
	{
		temp_a= (1- distancia/distancia_de_interaccion_max);
		temp_a*=temp_a;
		temp_a*=-fenotipo.mecanica.fuerza_de_adhesion_mb;
	}
	double temp_r = 0;
	if(distancia < fenotipo.geometria.radio)
	{
		temp_r = (1- distancia/fenotipo.geometria.radio);
		temp_r *= temp_r;
		temp_r *= fenotipo.mecanica.fuerza_de_repulsion_mb;
	}
	temp_r += temp_a;
	if( fabs( temp_r ) < 1e-16 )
	{ return; }

   velocidad.x += temp_r * desplazamiento.x;
	velocidad.y += temp_r * desplazamiento.y;
	velocidad.z += temp_r * desplazamiento.z;
	return;


}


void Celula::agregar_potenciales_mb_2(){

    if( pg->condiciones_de_periodicidad_x==true && pg->condiciones_de_periodicidad_y==true && pg->condiciones_de_periodicidad_z==true ){
		return;
	}
    std::cout << "Entre a calcular membrana \n";
	double distancia_de_interaccion_max = fenotipo.mecanica.distancia_de_adhesion_maxima_relativa * fenotipo.geometria.radio;

	double distancia;
	std::vector<double> distancias;
	std::vector<Vector> desplazamientos;

	desplazamiento.x = posicion.x - pg->rango_en_X[0];
	desplazamiento.y = 0;
	desplazamiento.z = 0;

	distancia = desplazamiento.x * desplazamiento.x;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_x){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

	desplazamiento.x = 0;
	desplazamiento.y = posicion.y - pg->rango_en_Y[0];
	desplazamiento.z = 0;

	distancia = desplazamiento.y * desplazamiento.y;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_y){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

	desplazamiento.x = 0;
	desplazamiento.y = 0;
	desplazamiento.z = posicion.z - pg->rango_en_Z[0];

	distancia = desplazamiento.z * desplazamiento.z;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_z){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

 	desplazamiento.x = pg->rango_en_X[1]- posicion.x;
	desplazamiento.y = 0;
	desplazamiento.z = 0;

	distancia = desplazamiento.x * desplazamiento.x;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_x){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

 	desplazamiento.x = 0;
	desplazamiento.y = pg->rango_en_Y[1]- posicion.y;
	desplazamiento.z = 0;

	distancia = desplazamiento.y * desplazamiento.y;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_y){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

	desplazamiento.x = 0;
	desplazamiento.y = 0;
	desplazamiento.z = pg->rango_en_Z[1]- posicion.z;

	distancia = desplazamiento.z * desplazamiento.z;
	distancia = std::max(sqrt(distancia), 0.00001);

	if(distancia < distancia_de_interaccion_max && !pg->condiciones_de_periodicidad_z){
	distancias.push_back(distancia);
	desplazamientos.push_back(desplazamiento);

	}

	if(distancias.size()>0){
        for(unsigned int i=0; i < distancias.size(); i++){

            double temp_a=0;
            temp_a= (1- distancias[i]/distancia_de_interaccion_max);
            temp_a*=temp_a;
            temp_a*=-fenotipo.mecanica.fuerza_de_adhesion_mb;

            double temp_r = 0;
            if(distancias[i] < fenotipo.geometria.radio){
                temp_r = (1- distancias[i]/fenotipo.geometria.radio);
                temp_r *= temp_r;
                temp_r *= fenotipo.mecanica.fuerza_de_repulsion_mb;
            }
            temp_r += temp_a;
            if( fabs( temp_r ) > 1e-16 )
            { axpy(&velocidad, temp_r, desplazamientos[i]); }


        }

	}

	return;


}


void Celula::actualizar_posicion( double dt ){

	static double d1;
	static double d2;
	static bool constants_defined = false;
	if( constants_defined == false )
	{
		d1 = dt;
		d1 *= 1.5;
		d2 = dt;
		d2 *= -0.5;
		constants_defined = true;
	}


   posicion.x += d1 * velocidad.x;
	posicion.y += d1 * velocidad.y;
	posicion.z += d1 * velocidad.z;
	
    posicion.x += d2 * velocidad_anterior.x;
	posicion.y += d2 * velocidad_anterior.y;
	posicion.z += d2 * velocidad_anterior.z;    

	velocidad_anterior = velocidad;

	velocidad.x = 0;
	velocidad.y = 0;
	velocidad.z = 0;

	set_posicion(posicion.x, posicion.y, posicion.z);
	actualizar_voxel_del_microambiente();

	return;

}


void Celula::comenzar_muerte( int indice_del_ciclo_de_muerte )
{

	fenotipo.muerte.comenzar_muerte(indice_del_ciclo_de_muerte);
	fenotipo.ciclo.sync_con_ciclo_modelo(fenotipo.muerte.ciclo_actual());
	fenotipo.ciclo.indice_de_la_fase_actual = 0;
	fenotipo.ciclo.tiempo_acumulado_en_la_fase = 0.0;
	fenotipo.secrecion.set_todas_las_secreciones_a_cero();
	fenotipo.secrecion.multiplicar_los_consumos_por_un_factor(0.10);

	if( fenotipo.ciclo.fase_actual().funcion_de_entrada)
	{
		fenotipo.ciclo.fase_actual().funcion_de_entrada(fenotipo.volumen, fenotipo.muerte.parametros_actuales());
	}

	return;
}



/*MICROAMBIENTE Y OXÍGENO*/

void Celula::registrar_microambiente( Microambiente* ){

	fenotipo.secrecion.tasas_de_secrecion.resize( microambiente->vector_de_densidades(0).size() , 0.0 );
	fenotipo.secrecion.densidades_de_saturacion.resize( microambiente->vector_de_densidades(0).size() , 0.0 );
	fenotipo.secrecion.tasas_de_consumo.resize( microambiente->vector_de_densidades(0).size() , 0.0 );
	fenotipo.secrecion.tasas_de_exportacion_neta.resize( microambiente->vector_de_densidades(0).size() , 0.0 );

	temp_celula_fuente_sumidero_solver1.resize( microambiente->vector_de_densidades(0).size() , 0.0 );
	temp_celula_fuente_sumidero_solver2.resize( microambiente->vector_de_densidades(0).size() , 1.0 );

	temp_celula_fuente_sumidero_exportacion_solver1.resize( microambiente->vector_de_densidades(0).size() , 0.0 );
	temp_celula_fuente_sumidero_exportacion_solver1.resize( microambiente->vector_de_densidades(0).size() , 0.0 );


	return;

}

void Celula::actualizar_voxel_del_microambiente(){

	if(!microambiente->mgrilla.es_valida_la_posicion(posicion.x, posicion.y, posicion.z)){
    std::cout << "Posicion invalida: " << posicion.x << " " << posicion.y << " " << posicion.z << " " << id << "\n ";
		voxel_del_microambiente =-1;
		es_activa =false;
		return;
	}

	voxel_del_microambiente=microambiente->indice_del_voxel_mas_cercano(posicion);

}

void Celula::set_constantes_de_consumo_interno( double dt ){

	double constante_interna_para_discretizar_la_aproximacion_delta = dt * fenotipo.volumen.total / ( (microambiente->voxeles(voxel_del_microambiente).volumen));

	temp_celula_fuente_sumidero_solver1.assign( (fenotipo.secrecion.tasas_de_secrecion).size() , 0.0 );
	temp_celula_fuente_sumidero_solver1 += fenotipo.secrecion.tasas_de_secrecion;
	temp_celula_fuente_sumidero_solver1 *= fenotipo.secrecion.densidades_de_saturacion;
	temp_celula_fuente_sumidero_solver1 *= constante_interna_para_discretizar_la_aproximacion_delta;

    temp_celula_fuente_sumidero_solver2.assign( (fenotipo.secrecion.tasas_de_secrecion).size() , 1.0 );
	axpy( &(temp_celula_fuente_sumidero_solver2) , constante_interna_para_discretizar_la_aproximacion_delta , fenotipo.secrecion.tasas_de_secrecion );
	axpy( &(temp_celula_fuente_sumidero_solver2) , constante_interna_para_discretizar_la_aproximacion_delta , fenotipo.secrecion.tasas_de_consumo );
	//std::cout << "temp2 = " << temp_celula_fuente_sumidero_solver2[0] << std::endl;

	temp_celula_fuente_sumidero_exportacion_solver1 = fenotipo.secrecion.tasas_de_exportacion_neta;
	temp_celula_fuente_sumidero_exportacion_solver1 *= dt; // amount exported in dt of time
	//std::cout << "temp_exp1 = " << temp_celula_fuente_sumidero_exportacion_solver1[0] << std::endl;

	temp_celula_fuente_sumidero_exportacion_solver2 = temp_celula_fuente_sumidero_exportacion_solver1;
	temp_celula_fuente_sumidero_exportacion_solver2 /= ( (microambiente->voxeles(voxel_del_microambiente)).volumen ) ;
	//std::cout << "temp_exp2 = " << temp_celula_fuente_sumidero_exportacion_solver2[0] << std::endl;

	fenotipo.volumen.cambio_el_volumen = false;

	return;


}

Microambiente* Celula::get_microambiente( void ){

	return microambiente;

}

int Celula::get_indice_del_voxel_del_microambiente( void )
{
	return voxel_del_microambiente;
}

std::vector<double>& Celula::vector_de_densidades_mas_cercano( void )
{
	return microambiente->vector_de_densidades_mas_cercano( voxel_del_microambiente );
}


std::vector<double>& Celula::gradiente_mas_cercano( int indice_del_sustrato ){

    return microambiente->vector_de_gradientes(voxel_del_microambiente)[indice_del_sustrato];

}


void Celula::simular_secrecion_y_consumo( double dt ){

	if(!es_activa)
	{ return; }

	if( fenotipo.volumen.cambio_el_volumen )
	{//std::cout << "seteo constantes \n";
		set_constantes_de_consumo_interno(dt);
		fenotipo.volumen.cambio_el_volumen = false;
	}


	(*microambiente)(voxel_del_microambiente) += temp_celula_fuente_sumidero_solver1;
	(*microambiente)(voxel_del_microambiente) /= temp_celula_fuente_sumidero_solver2;

	//aver = (*microambiente)(voxel_del_microambiente);

	//std::cout <<  aver[0] << std::endl;

	(*microambiente)(voxel_del_microambiente) += temp_celula_fuente_sumidero_exportacion_solver2;

	//std::cout <<  aver[0] << std::endl;


//	fenotipo.molecular.sustratos_totales_internalizados -= temp_celula_fuente_sumidero_exportacion_solver2;
//
//	std::cout <<  fenotipo.molecular.sustratos_totales_internalizados[0] << std::endl;
//
//	if(fenotipo.molecular.sustratos_totales_internalizados[0]<5){
//		es_activa=false;
//	}

	return;


}


void Celula::inicializar_celula(){
	//std::cout << "entre a incicializar celula \n";
	fenotipo.ciclo.sync_con_ciclo_modelo(ki67_cancer);
//	fenotipo.ciclo.sync_con_ciclo_modelo(vida);
	//std::cout << "sincronice el ciclo \n";
//	std::cout << "OXIGENOOOOOOOOOOO ID: " << oxigeno_ID << std::cout;
	static int oxigeno_ID = microambiente->encontrar_indice_de_densidad( "oxigeno" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[oxigeno_ID] = pg->tasas_de_secrecion;
	fenotipo.secrecion.tasas_de_consumo[oxigeno_ID] = pg->tasas_de_consumo;
	fenotipo.secrecion.densidades_de_saturacion[oxigeno_ID] = pg->densidades_de_saturacion;
/*	//FACTOR INMUNOESTIMULATORIO
    if( pg->activar_respuesta_inmune == true ){
    static int imm_ID = microambiente->encontrar_indice_de_densidad( "immunostimulatory factor" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[imm_ID] = 10.0;
	fenotipo.secrecion.densidades_de_saturacion[imm_ID] = 1.0;
	}
*/	
	    //Sustancia X
    if( pg->activar_sustancia_x == true ){
    static int X_ID = get_microambiente()->encontrar_indice_de_densidad( "sustancia x" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[X_ID] = 0.0;
	fenotipo.secrecion.densidades_de_saturacion[X_ID] = 0.0;
	}
	//std::cout << "nombre \n";
	nombre= pg->c_nombre;


    fenotipo.secrecion.oncoproteina = 1.0; //rng->NormalRandom();
    if( fenotipo.secrecion.oncoproteina < 0.0 ){
    fenotipo.secrecion.oncoproteina = 0.0; }

	parametros.o2_saturacion_para_la_proliferacion=pg->o2_saturacion_para_la_proliferacion;
	parametros.o2_referencia=pg->o2_referencia;
    parametros.o2_limite_de_proliferacion = 15.0; //Para la célula cancerosa en el hígado

	fenotipo.muerte.agregar_ciclo_de_muerte(0.0, &necrosis, necrosis_parametros);
	fenotipo.muerte.agregar_ciclo_de_muerte(0.0, &apoptosis, apoptosis_parametros);


	//std::cout << "actualizar voxel del microambiente \n";
	actualizar_voxel_del_microambiente();
	//std::cout << "si =) \n";
    
    //Cosas de tejido sano
    probabilidad= pg->probabilidad;
//    divisiones_permitidas=round(rng->RandomNumber( 1.0, 2.0 ));
    divisiones_permitidas= 1e9;//1-round(rng->RandomNumber() - 0.201); //Promedio de división 0.7
    
    fenotipo.volumen.citoplasma_tasa_de_cambio = 0.14/60.0;
	fenotipo.volumen.nucleo_tasa_de_cambio = 0.22/60.0;
	fenotipo.volumen.fluido_tasa_de_cambio = 1.4/60.0;


	return;
}

void Celula::mostrar_informacion_de_la_celula(std::ostream& os){

	os << " tipo:" << tipo << " nombre: " << nombre << std::endl;
	os << " radio:" << fenotipo.geometria.radio << " volumen: " << fenotipo.volumen.total << std::endl;
	os << " posicion: " << posicion << std::endl;
	os << " voxel_M: " << voxel_del_microambiente << std::endl;
	os << " voxel_C: " << voxel << std::endl;
	//Resumen del ciclo
	os << " cycle model: " << fenotipo.ciclo.pCiclo_Modelo->nombre
				<< " (codigo=" << fenotipo.ciclo.pCiclo_Modelo->codigo << ")" << std::endl;
	fenotipo.ciclo.pCiclo_Modelo->mostrar_ciclo(std::cout);

	//Secreciones
	for( unsigned int i = 0 ; i < fenotipo.secrecion.tasas_de_secrecion.size() ; i++ )
	{
		os << " tasa de secrecion =" << fenotipo.secrecion.tasas_de_secrecion[i] << std::endl;
		os << " tasa de consumo =" << fenotipo.secrecion.tasas_de_consumo[i] << std::endl;
		os << " densidades_de_saturacion =" << fenotipo.secrecion.densidades_de_saturacion[i] << std::endl;}

		return;

}


void Celula::inicializar_celula_sana(){
	//std::cout << "entre a incicializar celula sana \n";
	fenotipo.ciclo.sync_con_ciclo_modelo(vida);

////////Apagar la proliferación
	int indice_inicio_ciclo = vida.encontrar_indice_de_la_fase(c->viva);
	int indice_fin_ciclo = vida.encontrar_indice_de_la_fase(c->viva);
	fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_inicio_ciclo, indice_fin_ciclo) = 0.0;
	fenotipo.ciclo.tasas_de_transicion = fenotipo.ciclo.pCiclo_Modelo->tasas_de_transicion;
	//fenotipo.ciclo.pCiclo_Modelo->fases[0].actualizar_volumen = false;
////////

	static int oxigeno_ID = microambiente->encontrar_indice_de_densidad( "oxigeno" ); // 0
//	std::cout << "OXIGENOOOOOOOOOOO ID: " << oxigeno_ID << std::cout;
	fenotipo.secrecion.tasas_de_secrecion[oxigeno_ID] = 0.0;
	fenotipo.secrecion.tasas_de_consumo[oxigeno_ID] = 1.0;
	nombre= "Sana";
	tipo = 1;

	//fenotipo.mecanica.fuerza_de_adhesion_cc = 2.0;

    fenotipo.muerte.agregar_ciclo_de_muerte(0.0, &necrosis, necrosis_parametros);

	//std::cout << "actualizar voxel del microambiente \n";
	actualizar_voxel_del_microambiente();
	//std::cout << "si =) \n";

	return;
}

void Celula::inicializar_hepato(){
	//std::cout << "entre a incicializar celula \n";
    //divisiones_permitidas= 1-round(rng->RandomNumber() - 0.201); //Promedio de división 0.7
    if(divisiones_permitidas==0){
        fenotipo.ciclo.sync_con_ciclo_modelo(hepato_70);
        divisiones_permitidas=1; //En avanzar fenotipo necesito que sea 1, pero no afecta nada ya que el ciclo no le permite proliferar.
    }else{
        fenotipo.ciclo.sync_con_ciclo_modelo(Ki67);
    }
//	fenotipo.ciclo.sync_con_ciclo_modelo(vida);
	//std::cout << "sincronice el ciclo \n";
//	std::cout << "OXIGENOOOOOOOOOOO ID: " << oxigeno_ID << std::cout;
	static int oxigeno_ID = microambiente->encontrar_indice_de_densidad( "oxigeno" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[oxigeno_ID] = pg->tasas_de_secrecion;
	fenotipo.secrecion.tasas_de_consumo[oxigeno_ID] = pg->tasas_de_consumo;
	fenotipo.secrecion.densidades_de_saturacion[oxigeno_ID] = pg->densidades_de_saturacion;
/*	//FACTOR INMUNOESTIMULATORIO
    if( pg->activar_respuesta_inmune == true ){
    static int imm_ID = microambiente->encontrar_indice_de_densidad( "immunostimulatory factor" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[imm_ID] = 10.0;
	fenotipo.secrecion.densidades_de_saturacion[imm_ID] = 1.0;
	}
*/	
	    //Sustancia X
    if( pg->activar_sustancia_x == true ){
    static int X_ID = get_microambiente()->encontrar_indice_de_densidad( "sustancia x" ); // 0
	fenotipo.secrecion.tasas_de_secrecion[X_ID] = 10.0;
	fenotipo.secrecion.densidades_de_saturacion[X_ID] = 1.0;
	}
	//std::cout << "nombre \n";
	nombre= pg->c_nombre;
    tipo =1;


    fenotipo.secrecion.oncoproteina = rng->NormalRandom();
    if( fenotipo.secrecion.oncoproteina < 0.0 ){
    fenotipo.secrecion.oncoproteina = 0.0; }

	parametros.o2_saturacion_para_la_proliferacion=pg->o2_saturacion_para_la_proliferacion;
	parametros.o2_referencia=pg->o2_referencia;

	fenotipo.muerte.agregar_ciclo_de_muerte(0.0, &necrosis, necrosis_parametros);
	fenotipo.muerte.agregar_ciclo_de_muerte(0.0, &apoptosis, apoptosis_parametros);


	//std::cout << "actualizar voxel del microambiente \n";
	actualizar_voxel_del_microambiente();
	//std::cout << "si =) \n";
    
    //Cosas de tejido sano
    probabilidad= pg->probabilidad;
//    divisiones_permitidas=round(rng->RandomNumber( 1.0, 2.0 ));
    //divisiones_permitidas= 1-round(rng->RandomNumber() - 0.201); //Promedio de división 0.7


	return;
}


Linfocito::Linfocito(){

    Celula();
	//std::cout << "entre a incicializar celula sana \n";
	nombre= "Linfocito";
	tipo = 2;
    pg->numero_id += 1;
    id=pg->numero_id;
	fenotipo.ciclo.sync_con_ciclo_modelo(vida);

////////Apagar la proliferación
	int indice_inicio_ciclo = vida.encontrar_indice_de_la_fase(c->viva);
	int indice_fin_ciclo = vida.encontrar_indice_de_la_fase(c->viva);
    fenotipo.ciclo.actualizar_mis_tasas_de_transicion(indice_inicio_ciclo, indice_fin_ciclo) = 0.0;
//	fenotipo.ciclo.pCiclo_Modelo->tasa_de_transicion(indice_inicio_ciclo, indice_fin_ciclo) = 0.0;
//	fenotipo.ciclo.tasas_de_transicion = fenotipo.ciclo.pCiclo_Modelo->tasas_de_transicion;
////////

	static int oxigeno_ID = get_microambiente()->encontrar_indice_de_densidad( "oxigeno" ); 
	fenotipo.secrecion.tasas_de_secrecion[oxigeno_ID] = 0.0;
	fenotipo.secrecion.tasas_de_consumo[oxigeno_ID] = 1.0;

	fenotipo.muerte.agregar_ciclo_de_muerte(1.0 / (c->dt_ciclo * 10.0 * 24.0 * 60.0 ), &apoptosis, apoptosis_parametros);

	tasa_de_asesinato = 0.06667;
    tiempo_de_adhesion = 60.0;
    tasa_de_adhesion = 0.2; 
    constante_elastica = 0.01;
    distancia_de_adhesion_maxima = 18.0;
    distancia_de_adhesion_minima = 14.0;
    saturacion_de_oncoproteina = 2.0;
    limite_de_oncoproteina = 0.5;
    diferencia_de_oncoproteina = saturacion_de_oncoproteina - limite_de_oncoproteina;
    diferencia_de_adhesion = distancia_de_adhesion_maxima - distancia_de_adhesion_minima;
    hora_de_la_ultima_mecanica = pg->tiempo_total;
    hora_del_ultimo_ciclo = pg->tiempo_total;

//    celula_adherida = NULL;


	motilidad.es_movil = true;
	motilidad.tiempo_de_persistencia = 10.0;
	motilidad.velocidad_de_migracion = 5.0;
	motilidad.bias_de_la_migracion = 0.5;

	fenotipo.mecanica.fuerza_de_adhesion_cc = 0.0;
	fenotipo.mecanica.fuerza_de_adhesion_co = 0.0;
	fenotipo.mecanica.fuerza_de_adhesion_mb = 0.0;

	fenotipo.mecanica.fuerza_de_repulsion_cc *= 5.0;
	fenotipo.mecanica.fuerza_de_repulsion_co = fenotipo.mecanica.fuerza_de_repulsion_cc;



	//std::cout << "actualizar voxel del microambiente \n";
	actualizar_voxel_del_microambiente();
	//std::cout << "si =) \n";

	return;


}
void Linfocito::es_movil(bool valor){
    motilidad.es_movil = valor;
    return;
}

void Linfocito::actualizar_vector_de_motilidad( double dt, std::vector<Celula*> celulas_en_mi_voxel ){

	if( motilidad.es_movil == false )
	{
		motilidad.vector_de_motilidad.x = 0.0;
		motilidad.vector_de_motilidad.y = 0.0;
		motilidad.vector_de_motilidad.z = 0.0;
		return;
	}

	if( rng->RandomNumber() < dt / motilidad.tiempo_de_persistencia || motilidad.tiempo_de_persistencia < dt )
	{
		double temp_angle = 6.28318530717959*rng->RandomNumber();
		double temp_phi = 3.1415926535897932384626433832795*rng->RandomNumber();

		double sin_phi = sin(temp_phi);
		double cos_phi = cos(temp_phi);

		Vector randvec;
		randvec.x = sin_phi;
		randvec.y = sin_phi;
		randvec.z = sin_phi;

		randvec.x *= cos( temp_angle );
		randvec.y *= sin( temp_angle );
		randvec.z = cos_phi;

		motilidad_de_linfocito( dt, celulas_en_mi_voxel  ); //////////////////////////PROGRAMAR
		motilidad.vector_de_motilidad = motilidad.bias_de_la_migracion_direccion; 
		motilidad.vector_de_motilidad = motilidad.vector_de_motilidad * motilidad.bias_de_la_migracion;
		double uno_menos_bias = 1.0 - motilidad.bias_de_la_migracion;

		axpy( &(motilidad.vector_de_motilidad), uno_menos_bias, randvec );
		v->normalizame( &(motilidad.vector_de_motilidad) );
		motilidad.vector_de_motilidad = motilidad.vector_de_motilidad * motilidad.velocidad_de_migracion;
		velocidad = velocidad + motilidad.vector_de_motilidad;
	}
	return;

}

void Linfocito::motilidad_de_linfocito( double dt, std::vector<Celula*> celulas_en_mi_voxel  )
{

	static int indice_del_factor_inmune = get_microambiente()->encontrar_indice_de_densidad("immunostimulatory factor");
	if( celulas_en_mi_voxel.size() == 1 )
	{

		motilidad.es_movil = true;
		motilidad.bias_de_la_migracion_direccion.x = gradiente_mas_cercano(indice_del_factor_inmune)[0];
		motilidad.bias_de_la_migracion_direccion.y = gradiente_mas_cercano(indice_del_factor_inmune)[1];
		motilidad.bias_de_la_migracion_direccion.z = gradiente_mas_cercano(indice_del_factor_inmune)[2];
		v->normalizame( &( motilidad.bias_de_la_migracion_direccion ) );
	}
	else if(celulas_en_mi_voxel.size() > 1)
	{

        bool todos_linfos = true;
        for(unsigned int i=1; i < celulas_en_mi_voxel.size(); i++){
            if(celulas_en_mi_voxel[i]->tipo == 1){
                todos_linfos = false;
                motilidad.es_movil = false;
                continue;
            }
        }
            if(todos_linfos == true){
                motilidad.es_movil = true;
                motilidad.bias_de_la_migracion_direccion.x = gradiente_mas_cercano(indice_del_factor_inmune)[0];
                motilidad.bias_de_la_migracion_direccion.y = gradiente_mas_cercano(indice_del_factor_inmune)[1];
                motilidad.bias_de_la_migracion_direccion.z = gradiente_mas_cercano(indice_del_factor_inmune)[2];
                v->normalizame( &( motilidad.bias_de_la_migracion_direccion ) );

            }
	}

	return;
}


void Linfocito::avanzar_linfocito( double dt, std::vector<Celula*> celulas_en_mi_voxel )
{
	//static int tiempo_de_adhesion_c = tiempo_de_adhesion;
//    std::cout << "Avanzazr linfocito \n";


	if( celulas_en_mi_voxel.size() > 1 )
	{
		for(int i=0; i < celulas_en_mi_voxel.size(); i++ ){
            if(celulas_en_mi_voxel[i] != this && celulas_en_mi_voxel[i]->tipo != 2){
            Vector desplazamiento = celulas_en_mi_voxel[i]->posicion - posicion;
            axpy( &(velocidad) , constante_elastica , desplazamiento );
            }
		}


		bool soltarme = false;

		if( adherida && intento_de_apoptosis( celula_adherida, dt ) )
		{//std::cout << "Avanzazr linfocito 3\n";
			desencadenar_apoptosis( celula_adherida );
			//std::cout << "Avanzazr linfocito 3.1 \n";
			soltarme = true;

		}

		std::cout << "Avanzazr linfocito 4\n";

		if( adherida && rng->RandomNumber() < dt / ( tiempo_de_adhesion + 1e-15 ) )
		{ soltarme = true; }

//std::cout << "Avanzazr linfocito 5 \n";
		if( soltarme )
		{
//		std::cout << "Avanzazr linfocito 4 \n";
			soltar_celula( celula_adherida );
//			std::cout << "Avanzazr linfocito 4.1 \n";
			motilidad.es_movil = true;
		}
//		std::cout << "Avanzazr linfocito 7\n";
    //////////////////////////////////////////////return;
	//////////////////////////////////////////////}

	if( adherida==false){

//	std::cout << "Avanzazr linfocito 5 \n";
	bool check = chequear_vecinos_para_adherirse( celulas_en_mi_voxel , dt);
		if (check == true){
		motilidad.es_movil = false;
		return;
		}
	}
	motilidad.es_movil = true;
	}

	return;
}

bool Linfocito::intento_de_apoptosis( Celula* celula_adherida, double dt )
{
	if( celula_adherida->fenotipo.secrecion.oncoproteina < limite_de_oncoproteina )
	{ return false; }

	double escala = celula_adherida->fenotipo.secrecion.oncoproteina;
	escala -= limite_de_oncoproteina;
	escala /= diferencia_de_oncoproteina;
	if( escala > 1.0 )
	{ escala = 1.0; }


	if( rng->RandomNumber() < tasa_de_asesinato * escala * dt )
	{
		return true;
	}
	return false;
}


bool Linfocito::desencadenar_apoptosis( Celula* celula_adherida )
{
	static int indice_del_ciclo_apoptosis = celula_adherida->fenotipo.muerte.encontrar_indice_del_ciclo_de_muerte("Apoptosis");

//	std::cout << "Desencadenar apop en cel: " << celula_adherida->id << "\n";
	if( celula_adherida->fenotipo.muerte.muerta == true ){
  //      std::cout << "Falso \n";
        return false; }
	//std::cout << "Medio \n";
	celula_adherida->comenzar_muerte(indice_del_ciclo_apoptosis);
	//std::cout << "True \n";
	//std::cin.get();
	return true;
}

void Linfocito::adherir_celula(Celula* celula_objetivo){

    if(adherida==false && celula_objetivo->adherida == false){
        celula_adherida = celula_objetivo;
        adherida = true;
        celula_objetivo->adherida = true;
    }

 return;
}

void Linfocito::soltar_celula(Celula* celula_objetivo){

    if(adherida==true && celula_objetivo->adherida == true){
        celula_adherida = NULL;
        adherida = false;
        celula_objetivo->adherida = false;
    }

 return;
}

bool Linfocito::chequear_vecinos_para_adherirse( std::vector<Celula*> celulas_en_mi_voxel , double dt )
{
    unsigned int i = 0;
	while( i < celulas_en_mi_voxel.size() && adherida == false)
	{
	//	std::cout<<"Celulas en mi voxel = " << celulas_en_mi_voxel.size() << "\n";
	//	std::cout << "Chequear vecinos \n";
		if( celulas_en_mi_voxel[i] != this )
		{//std::cout << "Chequear vecinos 1 \n";
			if( intentar_adherirse( celulas_en_mi_voxel[i] , dt ) )
			{ //return celulas_en_mi_voxel[i];
			//std::cout << "Chequear vecinos 2\n";
                return true;
			}
		}
		i= i+1;
		//std::cout << i << "\n";
	}

	return false;
}

bool Linfocito::intentar_adherirse( Celula* celula_objetivo , double dt )
{
//    std::cout << "Intentar adherirse \n";

	if( celula_objetivo->fenotipo.secrecion.oncoproteina > limite_de_oncoproteina && celula_objetivo->fenotipo.muerte.muerta == false && celula_objetivo->adherida == false )
	{
		Vector desplazamiento = celula_objetivo->posicion - posicion;
		double escala_de_distancia = norma(desplazamiento);
//		std::cout << celula_objetivo->posicion << posicion << escala_de_distancia << "\n";
		if( escala_de_distancia > distancia_de_adhesion_maxima ){
  //          std::cout << "Intentar adherirse 1 \n";
            return false;
        }

		double escala = celula_objetivo->fenotipo.secrecion.oncoproteina;
		escala -= limite_de_oncoproteina;
		escala /= diferencia_de_oncoproteina;
		if( escala > 1.0 )
		{ escala = 1.0; }
	//	std::cout << "Intentar adherirse 2 \n";

		escala_de_distancia *= -1.0;
		escala_de_distancia += distancia_de_adhesion_maxima;
		escala_de_distancia /= diferencia_de_adhesion;
		if( escala_de_distancia > 1.0 )
		{ escala_de_distancia = 1.0; }
	//	std::cout << "Intentar adherirse 3 \n";

		if( rng->RandomNumber() < tasa_de_adhesion * escala * dt * escala_de_distancia )
		{
	//	std::cout << "Intentar adherirse 4 \n";
            adherir_celula(celula_objetivo);
      //      std::cout << "Intentar adherirse 5 \n";
            return true;
		}


	}

	return false;
}

