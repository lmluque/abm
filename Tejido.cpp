#include "Tejido.h"

Tejido::Tejido(){

	std::cout << "entre a tejido \n";
//	microambiente;
//	cdc;
}

void Tejido::inicializar_tejido(){

    //Microambiente
	microambiente.inicializar_microambiente();

	//Contenedor de células
	cdc.inicializar(pg->rango_en_X[0],pg->rango_en_X[1],pg->rango_en_Y[0],pg->rango_en_Y[1],
	pg->rango_en_Z[0],pg->rango_en_Z[1], pg->c_dx, pg->c_dy, pg->c_dz);

    //Célula Cancerosa
	cdc.celula = crear_celula();
	cdc.celula->inicializar_hepato();
    cdc.celula->fenotipo.secrecion.oncoproteina = 1.0;//rng->NormalRandom_CM( 1.0, 0.01 );
	cdc.registrar_celula(cdc.celula);
	celulas_para_registrar_en_voxeles.clear();

	//Planos de células sanas
	std::vector<Vector> posiciones = crear_esfera_de_celulas(200);
	Celula* pCelula = NULL;


    for( long unsigned int i=0; i < posiciones.size(); i++ ){
        if(i==0){
            cdc.celula->set_posicion( posiciones[i] );
            cdc.registrar_celula(cdc.celula);
            celulas_para_registrar_en_voxeles.clear();            
        }else{

            pCelula = crear_celula();
            pCelula->inicializar_hepato();
            pCelula->set_posicion( posiciones[i] );
            pCelula->fenotipo.secrecion.oncoproteina = 1.0;//rng->NormalRandom_CM( 1.0, 0.01 );
            cdc.registrar_celula(pCelula);
            celulas_para_registrar_en_voxeles.clear();
            }

    }
    
    std::vector<Vector> nodos_del_plano = crear_plano_de_vs();
    microambiente.posibles_nodos(nodos_del_plano);
    microambiente.vasos_del_higado(200, nodos_del_plano);
    
//     for(int i=0; i < microambiente.posibles_voxeles_del_vaso_sanguineo.size(); i++){
//         
//         std::cout << microambiente.posibles_voxeles_del_vaso_sanguineo[i] << "\n";
//     }
//     
//     std::cout << "Entro al segundo FOR \n";
//     
//     for(int i=0; i < microambiente.voxeles_del_vaso_sanguineo.size(); i++){
//         
//         std::cout << microambiente.voxeles_del_vaso_sanguineo[i] << "\n";
//     }    
//     std::cin.get();

	return;


}


std::vector<Vector> Tejido::crear_planos_de_celulas_sanas_en_Z(int cantidad, double posicion_en_z_del_primer_plano){

	std::vector<Vector> posiciones;
    int xc=0,yc=0,zc=0;
	double espacio_en_x = cdc.celula->fenotipo.geometria.radio*sqrt(3);
	double espacio_en_y = cdc.celula->fenotipo.geometria.radio*2;
	double espacio_en_z = cdc.celula->fenotipo.geometria.radio*sqrt(3);

    Vector punto_temporal;

	for(double z=0; z<=cantidad; z++, zc++)
	{
		for(double x=pg->rango_en_X[0]; x<pg->rango_en_X[1]; x+=espacio_en_x, xc++)
		{
			for(double y=pg->rango_en_Y[0]; y<pg->rango_en_Y[1]; y+=espacio_en_y, yc++)
			{
				punto_temporal.x= x + (zc%2) * 0.5 * cdc.celula->fenotipo.geometria.radio;
				punto_temporal.y= y + (xc%2) * cdc.celula->fenotipo.geometria.radio;
				punto_temporal.z= posicion_en_z_del_primer_plano + z*espacio_en_z;

				posiciones.push_back(punto_temporal);
			}

		}
	}
	return posiciones;



}

std::vector<Vector> Tejido::crear_esfera_de_celulas(double radio_de_la_esfera){

	std::vector<Vector> posiciones;
	int xc=0,yc=0,zc=0;
	double espacio_en_x = cdc.celula->fenotipo.geometria.radio*sqrt(3);
	double espacio_en_y = cdc.celula->fenotipo.geometria.radio*2;
	double espacio_en_z = cdc.celula->fenotipo.geometria.radio*sqrt(3);

	Vector punto_temporal;

	for(double z=-radio_de_la_esfera;z<radio_de_la_esfera;z+=espacio_en_z, zc++)
	{
		for(double x=-radio_de_la_esfera;x<radio_de_la_esfera;x+=espacio_en_x, xc++)
		{
			for(double y=-radio_de_la_esfera;y<radio_de_la_esfera;y+=espacio_en_y, yc++)
			{
				punto_temporal.x=x + (zc%2) * 0.5 * cdc.celula->fenotipo.geometria.radio;
				punto_temporal.y=y + (xc%2) * cdc.celula->fenotipo.geometria.radio;
				punto_temporal.z=z;

				if(sqrt(punto_temporal.x*punto_temporal.x+punto_temporal.y*punto_temporal.y+punto_temporal.z*punto_temporal.z)< radio_de_la_esfera)
				{ posiciones.push_back(punto_temporal); }
			}

		}
	}
	return posiciones;


}


void Tejido::introducir_linfocitos(int cantidad){

	double radio_del_tumor_b = -9e9; // 250.0;
	double radio_temporal_b = 0.0;

	for( unsigned int i=0; i < todas_las_celulas.size() ; i++ )
	{
		radio_temporal_b = norm_squared( todas_las_celulas[i]->posicion );
		if( radio_temporal_b > radio_del_tumor_b )
		{ radio_del_tumor_b = radio_temporal_b; }
	}
	radio_del_tumor_b = sqrt( radio_del_tumor_b );


	double radio_interno = radio_del_tumor_b + 30.0;
	double radio_externo = radio_interno + 75.0;

	double radio_medio = 0.5*(radio_interno + radio_externo);
	double radio_std = 0.33*( radio_externo-radio_interno)/2.0;
    Celula* pLinfocito = NULL;

	for( int i=0 ;i < cantidad ; i++ )
	{
		double theta = rng->RandomNumber() * 6.283185307179586476925286766559;
		double phi = acos( 2.0*rng->RandomNumber() - 1.0 );

		double radio = rng->NormalRandom_CM( radio_medio, radio_std );


		pLinfocito = new Linfocito;
		todas_las_celulas.push_back(pLinfocito);
		celulas_para_registrar_en_voxeles.push_back(pLinfocito);
		pLinfocito->indice=todas_las_celulas.size()-1;
		//pLinfocito->id=todas_las_celulas.size();
    	//pLinfocito->inicializar_linfocito();
    	pLinfocito->set_posicion(radio*cos(theta)*sin(phi), radio*sin(theta)*sin(phi), radio*cos(phi));

    	cdc.registrar_celula(pLinfocito);
    	celulas_para_registrar_en_voxeles.clear();
	}

	return;



}


void Tejido::introducir_linfocitos_aleatorios(int cantidad){

	double radio_del_tumor_b = -9e9;
	double radio_temporal_b = 0.0;
    Vector pos_temp;
    double radio_temp_2 = 0.0;

	for( unsigned int i=0; i < todas_las_celulas.size() ; i++ )
	{
        if( todas_las_celulas[i]->tipo == 0){
            radio_temporal_b = norm_squared( todas_las_celulas[i]->posicion );
            if( radio_temporal_b > radio_del_tumor_b )
            { radio_del_tumor_b = radio_temporal_b; }
        }
    }   
	radio_del_tumor_b = sqrt( radio_del_tumor_b );
	double radio_interno = radio_del_tumor_b + 50.0;

    

    Celula* pLinfocito = NULL;

	for( int i=0 ;i < cantidad ; i++ )
	{

        do{
            pos_temp.x = rng->RandomNumber(pg->rango_en_X[0],pg->rango_en_X[1]);
            pos_temp.y = rng->RandomNumber(pg->rango_en_Y[0],pg->rango_en_Y[1]);
            pos_temp.z = rng->RandomNumber(pg->rango_en_Z[0],pg->rango_en_Z[1]);
            radio_temp_2 = sqrt(pos_temp.x*pos_temp.x + pos_temp.y*pos_temp.y + pos_temp.z*pos_temp.z);
        }while(radio_interno > radio_temp_2);
        
        
        
        if(radio_interno < radio_temp_2){
            pLinfocito = new Linfocito;
            todas_las_celulas.push_back(pLinfocito);
            celulas_para_registrar_en_voxeles.push_back(pLinfocito);
            pLinfocito->indice=todas_las_celulas.size()-1;
            pLinfocito->set_posicion(pos_temp.x, pos_temp.y, pos_temp.z);
            cdc.registrar_celula(pLinfocito);
            celulas_para_registrar_en_voxeles.clear();
        }
	}

	return;



}

void Tejido::geometria_del_tumor(){
    
 	radio_del_tumor = -9e9; 
	double radio_temporal = 0.0;
    volumen_del_tumor = 0.0;
    volumen_del_tumor2 = 0.0;
    celulas_tumorales = 0;
    celulas_muertas = 0;
    static double cuatrotercios = 1.33333333333333333333;
    static double pi = 3.1415926535897932384626433832795;

	for( unsigned int i=0; i < todas_las_celulas.size() ; i++ )
	{
        if( todas_las_celulas[i]->tipo == 0){
            radio_temporal = norm_squared( todas_las_celulas[i]->posicion );
            volumen_del_tumor2 += todas_las_celulas[i]->fenotipo.volumen.total;
            if( radio_temporal > radio_del_tumor ){
                radio_del_tumor = radio_temporal;                
            }
            celulas_tumorales += 1;
            if(todas_las_celulas[i]->fenotipo.muerte.muerta){
                celulas_muertas +=1;
            }
        }
	}
	radio_del_tumor = sqrt( radio_del_tumor );
    
    volumen_del_tumor = cuatrotercios * pi * radio_del_tumor * radio_del_tumor * radio_del_tumor;
    
    return;
    
}

void Tejido::cortar_higado(int PHx){

    int cantidad_de_celulas = todas_las_celulas.size();
    int celulas_a_sacar = round((PHx*cantidad_de_celulas)/100);
    
    //std::cout << "cant de celulas: " << cantidad_de_celulas << " celulas a sacar: " << celulas_a_sacar << "\n";
    Vector posicion_temp;
    double ciclo_temp;
    double mec_temp;
    Celula* pCelula = NULL;
    
    for(int j = 0; j<= celulas_a_sacar; j++){
        if(j==celulas_a_sacar){
            posicion_temp = todas_las_celulas[cantidad_de_celulas - j]->posicion;
            ciclo_temp=todas_las_celulas[cantidad_de_celulas - j]->hora_del_ultimo_ciclo;
            mec_temp=todas_las_celulas[cantidad_de_celulas - j]->hora_de_la_ultima_mecanica;
        }
                
        todas_las_celulas.pop_back();        
    }

    int indice_ultimo_voxel = todas_las_celulas[todas_las_celulas.size()-1]->voxel_del_microambiente;
    
    for(int j=microambiente.voxeles_del_vaso_sanguineo.size()-1; j>=0; j--){
        
        if(microambiente.voxeles_del_vaso_sanguineo[j] > indice_ultimo_voxel){
            
            microambiente.voxeles(microambiente.voxeles_del_vaso_sanguineo[j]).es_dirichlet=false;
            microambiente.voxeles_del_vaso_sanguineo.pop_back();
            
        }        
        
    }
    
        //AGREGAR CELULA CANCEROSA
    pCelula = crear_celula();
    pCelula->inicializar_celula();
   	pCelula->hora_del_ultimo_ciclo = ciclo_temp;
	pCelula->hora_de_la_ultima_mecanica = mec_temp;
    pCelula->set_posicion( posicion_temp );
    pCelula->fenotipo.secrecion.oncoproteina = 1.0;//rng->NormalRandom_CM( 1.0, 0.01 );
    cdc.registrar_celula(pCelula);
    celulas_para_registrar_en_voxeles.clear();
    //////////////////////////////////////////
    
//     std::cout << "CORTE EL VASO SANGUINEO \n";
//     for(int i=0; i < microambiente.voxeles_del_vaso_sanguineo.size(); i++){
//         
//         std::cout << microambiente.voxeles_del_vaso_sanguineo[i] << "\n";
//     }    
//     std::cin.get();

    //std::cout << "cant de celulas: " << todas_las_celulas.size() << "\n";
}


std::vector<Vector> Tejido::crear_plano_de_vs(){

	std::vector<Vector> voxeles_dirichlet;
	int x0=4;
	int y0=2;
    int z0=0;
    
    Vector punto_temporal;
    
    for(int x= x0; x<microambiente.mgrilla.coordenadas_x.size(); x=x+12){
        for(int y=y0; y<microambiente.mgrilla.coordenadas_y.size(); y=y+8){
            punto_temporal.x=x;
            punto_temporal.y=y;
            punto_temporal.z=z0;
            voxeles_dirichlet.push_back(punto_temporal);
            
            if(x<microambiente.mgrilla.coordenadas_x.size()-4){
                punto_temporal.x=x+4;
                punto_temporal.y=y;
                punto_temporal.z=z0;
                voxeles_dirichlet.push_back(punto_temporal);
            }
            
        }
    }
    
    
    for(int x2= x0; x2<microambiente.mgrilla.coordenadas_x.size()+2; x2=x2+14){
        
        x2=x2-2;
        for(int y2=y0+4; y2<microambiente.mgrilla.coordenadas_y.size(); y2=y2+8){

            
            if(x2 >= 0){
                punto_temporal.x=x2;
                punto_temporal.y=y2;
                punto_temporal.z=z0;
                voxeles_dirichlet.push_back(punto_temporal);
            }
            //Si no le agrego (long long) y (unsigned) hay un error en la comparación
            if((long long)x2<(unsigned)(microambiente.mgrilla.coordenadas_x.size()-8)){
                punto_temporal.x=x2+8;
                punto_temporal.y=y2;
                punto_temporal.z=z0;
                voxeles_dirichlet.push_back(punto_temporal);
            }
        }
    }
    

	return voxeles_dirichlet;

}


void Tejido::controlar_vasos_sanguineos(){
    
    for(int i=0; i<microambiente.posibles_voxeles_del_vaso_sanguineo.size();i++){
        
        if(microambiente.voxeles(microambiente.posibles_voxeles_del_vaso_sanguineo[i]).es_dirichlet==false){
            
            for(int j=0; j<todas_las_celulas.size(); j++){
                
                if(microambiente.voxeles(microambiente.posibles_voxeles_del_vaso_sanguineo[i]).es_dirichlet==false && todas_las_celulas[j]->voxel_del_microambiente == microambiente.posibles_voxeles_del_vaso_sanguineo[i]){
                    
                    std::cout << "Agrego voxel " << microambiente.posibles_voxeles_del_vaso_sanguineo[i] << "\n";
                    //std::cin.get();
                    
                    microambiente.voxeles_del_vaso_sanguineo.push_back(microambiente.posibles_voxeles_del_vaso_sanguineo[i]);
                    
                    microambiente.agregar_nodo_de_dirichlet(microambiente.posibles_voxeles_del_vaso_sanguineo[i], pg->vector_condicion_de_dirichlet);
                    
                    microambiente.set_activacion_de_sustrato_de_dirichlet(microambiente.posibles_voxeles_del_vaso_sanguineo[i], pg->dirichlet_vs);
                    
                }                                
            }
        }
    }
}
