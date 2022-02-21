#include "Microambiente.h"



Microambiente* microambiente_default = NULL;

void set_microambiente_default( Microambiente* M ){

	microambiente_default = M;

}

Microambiente* get_microambiente_default( void ){

	return microambiente_default;

}


Microambiente::Microambiente(){

	nombre = "unnamed";
	unidades_espaciales = "none";
	unidades_temporales = "none";

	setup_del_solver_bulk_fuente_sumidero_hecho = false;
	setup_de_thomas_hecho = false;
	setup_del_solver_de_difusion_hecho = false;

	mgrilla.redimensionar(1,1,1);

	uno.resize( 1 , 1.0 );
	cero.resize( 1 , 0.0 );

	temp_vectores_densidad1.resize( mgrilla.voxeles.size() , cero );
	temp_vectores_densidad2.resize( mgrilla.voxeles.size() , cero );
	p_vectores_densidad = &temp_vectores_densidad1;

	vectores_gradiente.resize( mgrilla.voxeles.size() );
	for( unsigned int k=0 ; k < mgrilla.voxeles.size() ; k++ )
	{
		vectores_gradiente[k].resize( 1 );
		(vectores_gradiente[k])[0].resize( 3, 0.0 );
	}
	vector_gradiente_calculado.resize( mgrilla.voxeles.size() , false );

	densidades_nombres.assign( 1 , "unnamed" );
	densidades_unidades.assign( 1 , "none" );

	coeficientes_de_difusion.assign( numero_de_densidades() , 0.0 );
	tasas_de_decaimiento.assign( numero_de_densidades() , 0.0 );

	un_medio = uno;
	un_medio *= 0.5;

	un_tercio = uno;
	un_tercio /= 3.0;

	vector_valores_de_dirichlet.assign( mgrilla.voxeles.size(), uno );
	vector_activacion_dirichlet.assign( 1 , true );

	dirichlet_vectorES_activacion.assign( 1 , vector_activacion_dirichlet );
/*
	pg->dirichlet_todo.assign( 1 , true );
	pg->dirichlet_xmin.assign( 1 , true );
	pg->dirichlet_xmax.assign( 1 , true );
	pg->dirichlet_ymin.assign( 1 , true );
	pg->dirichlet_ymax.assign( 1 , true );
	pg->dirichlet_zmin.assign( 1 , true );
	pg->dirichlet_zmax.assign( 1 , true );
	pg->dirichlet_vs.assign( 1 , true );
*/
    pg->dirichlet_xmin_valores.assign( 1 , 1.0 );
	pg->dirichlet_xmax_valores.assign( 1 , 1.0 );
	pg->dirichlet_ymin_valores.assign( 1 , 1.0 );
	pg->dirichlet_ymax_valores.assign( 1 , 1.0 );
	pg->dirichlet_zmin_valores.assign( 1 , 1.0 );
	pg->dirichlet_zmax_valores.assign( 1 , 1.0 );

	if(microambiente_default==NULL){
		microambiente_default=this;
	}

	return;

}

unsigned int Microambiente::numero_de_densidades( void ){

	return (*p_vectores_densidad)[0].size();
}

unsigned int Microambiente::numero_de_voxeles( void ){

	return mgrilla.voxeles.size();
}

void Microambiente::redimensionar_espacio( double x_ini, double x_fin, double y_ini, double y_fin, double z_ini, double z_fin , double dx_nuevo , double dy_nuevo , double dz_nuevo ){

	mgrilla.redimensionar(x_ini, x_fin, y_ini, y_fin, z_ini, z_fin , dx_nuevo, dy_nuevo, dz_nuevo );

	temp_vectores_densidad1.assign( mgrilla.voxeles.size() , cero );
	temp_vectores_densidad2.assign( mgrilla.voxeles.size() , cero );

	vectores_gradiente.resize( mgrilla.voxeles.size() );
	for( unsigned int k=0 ; k < mgrilla.voxeles.size() ; k++ )
	{
		vectores_gradiente[k].resize( numero_de_densidades() );
		for( unsigned int i=0 ; i < numero_de_densidades() ; i++ )
		{
			(vectores_gradiente[k])[i].resize( 3, 0.0 );
		}
	}
	vector_gradiente_calculado.resize( mgrilla.voxeles.size() , false );

	vector_valores_de_dirichlet.assign( mgrilla.voxeles.size(), uno );

	dirichlet_vectorES_activacion.assign( mgrilla.voxeles.size() , vector_activacion_dirichlet );

	return;
}

void Microambiente::redimensionar_densidades( int nuevo_tamano ){

	cero.assign( nuevo_tamano, 0.0 );
	uno.assign( nuevo_tamano, 1.0 );

	temp_vectores_densidad1.assign( mgrilla.voxeles.size() , cero );
	temp_vectores_densidad2.assign( mgrilla.voxeles.size() , cero );

	for( unsigned int k=0 ; k < mgrilla.voxeles.size() ; k++ )
	{
		vectores_gradiente[k].resize( numero_de_densidades() );
		for( unsigned int i=0 ; i < numero_de_densidades() ; i++ )
		{
			(vectores_gradiente[k])[i].resize( 3, 0.0 );
		}
	}
	vector_gradiente_calculado.resize( mgrilla.voxeles.size() , false );

	coeficientes_de_difusion.assign( nuevo_tamano , 0.0 );
	tasas_de_decaimiento.assign( nuevo_tamano , 0.0 );

	densidades_nombres.assign( nuevo_tamano, "unnamed" );
	densidades_unidades.assign( nuevo_tamano , "none" );

	un_medio = uno;
	un_medio *= 0.5;

	un_tercio = uno;
	un_tercio /= 3.0;

	vector_valores_de_dirichlet.assign( mgrilla.voxeles.size(), uno );
	vector_activacion_dirichlet.assign( nuevo_tamano, true );

	dirichlet_vectorES_activacion.assign(mgrilla.voxeles.size(), vector_activacion_dirichlet );

	pg->vector_condicion_de_dirichlet.assign( nuevo_tamano , 1.0 );
	pg->vector_activacion_dirichlet.assign( nuevo_tamano, true );

	pg->vector_condiciones_iniciales.assign( nuevo_tamano , 1.0 );

	pg->dirichlet_todo.assign( nuevo_tamano , true );

	pg->dirichlet_xmin.assign( nuevo_tamano , false );
	pg->dirichlet_xmax.assign( nuevo_tamano , false );
	pg->dirichlet_ymin.assign( nuevo_tamano , false );
	pg->dirichlet_ymax.assign( nuevo_tamano , false );
	pg->dirichlet_zmin.assign( nuevo_tamano , false );
	pg->dirichlet_zmax.assign( nuevo_tamano , false );
	pg->dirichlet_vs.assign( nuevo_tamano , false );

	pg->dirichlet_xmin_valores.assign( nuevo_tamano , 1.0 );
	pg->dirichlet_xmax_valores.assign( nuevo_tamano , 1.0 );
	pg->dirichlet_ymin_valores.assign( nuevo_tamano , 1.0 );
	pg->dirichlet_ymax_valores.assign( nuevo_tamano , 1.0 );
	pg->dirichlet_zmin_valores.assign( nuevo_tamano , 1.0 );
	pg->dirichlet_zmax_valores.assign( nuevo_tamano , 1.0 );

	return;


}

void Microambiente::agregar_densidad(std::string nombre , std::string unidades, double coeficiente_de_difusion, double tasa_de_decaimiento ){

	cero.push_back( 0.0 );
	uno.push_back( 1.0 );

	densidades_nombres.push_back( nombre );
	densidades_unidades.push_back( unidades );

	coeficientes_de_difusion.push_back( coeficiente_de_difusion );
	tasas_de_decaimiento.push_back( tasa_de_decaimiento );

	for( unsigned int i=0; i < temp_vectores_densidad1.size() ; i++ )
	{
		temp_vectores_densidad1[i].push_back( 0.0 );
		temp_vectores_densidad2[i].push_back( 0.0 );
	}

	for( unsigned int k=0 ; k < mgrilla.voxeles.size() ; k++ )
	{
		vectores_gradiente[k].resize( numero_de_densidades() );
		for( unsigned int i=0 ; i < numero_de_densidades(); i++ )
		{
			(vectores_gradiente[k])[i].resize( 3, 0.0 );
		}
	}
	vector_gradiente_calculado.resize( mgrilla.voxeles.size() , false );

	un_medio = uno;
	un_medio *= 0.5;


	un_tercio = uno;
	un_tercio /= 3.0;

	vector_valores_de_dirichlet.assign( mgrilla.voxeles.size(), uno );
	vector_activacion_dirichlet.push_back(true );
	dirichlet_vectorES_activacion.assign(mgrilla.voxeles.size(), vector_activacion_dirichlet );

	pg->vector_condicion_de_dirichlet.push_back( 1.0 ); 
	pg->vector_activacion_dirichlet.push_back( true ); 

	pg->vector_condiciones_iniciales.push_back( 1.0 );

	pg->dirichlet_todo.push_back( false );

	pg->dirichlet_xmin.push_back( false );
	pg->dirichlet_xmax.push_back( false );
	pg->dirichlet_ymin.push_back( false );
	pg->dirichlet_ymax.push_back( false );
	pg->dirichlet_zmin.push_back( false );
	pg->dirichlet_zmax.push_back( false );
	pg->dirichlet_vs.push_back( false );

	pg->dirichlet_xmin_valores.push_back( 1.0 );
	pg->dirichlet_xmax_valores.push_back( 1.0 );
	pg->dirichlet_ymin_valores.push_back( 1.0 );
	pg->dirichlet_ymax_valores.push_back( 1.0 );
	pg->dirichlet_zmin_valores.push_back( 1.0 );
	pg->dirichlet_zmax_valores.push_back( 1.0 );

	return;


}

void Microambiente::set_densidad(int indice, std::string nombre , std::string unidades , double coeficiente_de_difusion , double tasa_de_decaimiento ){

	if( indice == 0 )
	{ pg->usar_oxigeno_como_primer_sustrato = false; }

	densidades_nombres[indice] = nombre;
	densidades_unidades[indice] = unidades;

	coeficientes_de_difusion[indice] = coeficiente_de_difusion;
	tasas_de_decaimiento[indice] = tasa_de_decaimiento;
	return;


}

void Microambiente::set_densidad( int indice , std::string nombre , std::string unidades ){
	if( indice == 0 )
	{ pg->usar_oxigeno_como_primer_sustrato = false; }

	densidades_nombres[indice] = nombre;
	densidades_unidades[indice] = unidades;
}

int Microambiente::encontrar_indice_de_densidad( std::string nombre ){

	for( unsigned int i=0; i < densidades_nombres.size() ; i++ )
	{
		if( densidades_nombres[i] == nombre )
		{ return i; }
	}
	return -1;
}

int Microambiente::indice_de_voxel( int i, int j, int k ){

	return mgrilla.indice_de_voxel(i,j,k) ;
}

int Microambiente::indice_de_voxel( Vector& indices_cartesianos ){

	return mgrilla.indice_de_voxel(indices_cartesianos.x,indices_cartesianos.y,indices_cartesianos.z) ;
}


Vector Microambiente::centro_del_voxel(int indice_del_voxel){

    return mgrilla.get_centro_voxel(indice_del_voxel);

}


std::vector<unsigned int> Microambiente::indices_cartesianos( int n ){

	return mgrilla.indices_cartesianos( n );

}

int Microambiente::indice_del_voxel_mas_cercano( Vector& posicion ){

	return mgrilla.indice_del_voxel_mas_cercano( posicion );

}

Vector Microambiente::indices_cartesianos_mas_cercanos( Vector& posicion ){

	return mgrilla.indices_cartesianos_mas_cercanos( posicion );
}

Voxel& Microambiente::voxel_mas_cercano( Vector& posicion ){

	return mgrilla.voxel_mas_cercano( posicion );

}

Voxel& Microambiente::voxeles( int indice_de_voxel ){

	return mgrilla.voxeles[indice_de_voxel];

}

std::vector<double>& Microambiente::vector_de_densidades_mas_cercano( Vector& posicion ){

	return (*p_vectores_densidad)[ mgrilla.indice_del_voxel_mas_cercano( posicion ) ];

}

std::vector<double>& Microambiente::vector_de_densidades_mas_cercano( int indice_de_voxel ){

	return (*p_vectores_densidad)[ indice_de_voxel ];

}


std::vector<double>& Microambiente::operator()( int n ){

	return (*p_vectores_densidad)[ n ];

}

std::vector<double>& Microambiente::vector_de_densidades( int n ){

	return (*p_vectores_densidad)[ n ];

}

/////////////////////GRADIENTES//////////////////////////////
std::vector<gradiente>& Microambiente::vector_de_gradientes(int i, int j, int k){

    int n = fg_indice_de_voxel(i,j,k);
	if( vector_gradiente_calculado[n] == false )
	{
		calcular_vector_de_gradiente( n );
	}

	return vectores_gradiente[n];

}

std::vector<gradiente>& Microambiente::vector_de_gradientes(int n ){

	if( vector_gradiente_calculado[n] == false )
	{
		calcular_vector_de_gradiente( n );
	}


	return vectores_gradiente[n];
}


std::vector<gradiente>& Microambiente::vector_de_gradiente_mas_cercano( Vector& posicion ){

	int n = indice_del_voxel_mas_cercano( posicion );
	if( vector_gradiente_calculado[n] == false )
	{
		calcular_vector_de_gradiente( n );
	}

	return vectores_gradiente[n];

}

void Microambiente::calcular_todos_los_vectores_de_gradientes( void ){

	static double dos_dx = mgrilla.dx;
	static double dos_dy = mgrilla.dy;
	static double dos_dz = mgrilla.dz;
	static bool constantes_de_gradientes_definidas = false;
	if( constantes_de_gradientes_definidas == false )
	{
		dos_dx *= 2.0;
		dos_dy *= 2.0;
		dos_dz *= 2.0;
		constantes_de_gradientes_definidas = true;
	}

	#pragma omp parallel for
	for( unsigned int k=0; k < mgrilla.coordenadas_z.size(); k++ )
	{
		for( unsigned int j=0; j < mgrilla.coordenadas_y.size(); j++ )
		{
			// endcaps
			for( unsigned int q=0; q < numero_de_densidades(); q++ )
			{
				int i = 0;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][0] = (*p_vectores_densidad)[n+thomas_salto_en_i][q];
				vectores_gradiente[n][q][0] -= (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][0] /= mgrilla.dx;

				vector_gradiente_calculado[n] = true;
			}
			for( unsigned int q=0; q < numero_de_densidades() ; q++ )
			{
				int i = mgrilla.coordenadas_x.size()-1;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][0] = (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][0] -= (*p_vectores_densidad)[n-thomas_salto_en_i][q];
				vectores_gradiente[n][q][0] /= mgrilla.dx;

				vector_gradiente_calculado[n] = true;
			}

			for( unsigned int i=1; i < mgrilla.coordenadas_x.size()-1 ; i++ )
			{
				for( unsigned int q=0; q < numero_de_densidades() ; q++ )
				{
					int n = fg_indice_de_voxel(i,j,k);
					vectores_gradiente[n][q][0] = (*p_vectores_densidad)[n+thomas_salto_en_i][q];
					vectores_gradiente[n][q][0] -= (*p_vectores_densidad)[n-thomas_salto_en_i][q];
					vectores_gradiente[n][q][0] /= dos_dx;

					vector_gradiente_calculado[n] = true;
 				}
			}

		}
	}

	#pragma omp parallel for
	for( unsigned int k=0; k < mgrilla.coordenadas_z.size() ; k++ )
	{
		for( unsigned int i=0; i < mgrilla.coordenadas_x.size() ; i++ )
		{
			// endcaps
			for( unsigned int q=0; q < numero_de_densidades() ; q++ )
			{
				int j = 0;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][1] = (*p_vectores_densidad)[n+thomas_salto_en_j][q];
				vectores_gradiente[n][q][1] -= (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][1] /= mgrilla.dy;

				vector_gradiente_calculado[n] = true;
			}
			for( unsigned int q=0; q < numero_de_densidades() ; q++ )
			{
				int j = mgrilla.coordenadas_y.size()-1;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][1] = (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][1] -= (*p_vectores_densidad)[n-thomas_salto_en_j][q];
				vectores_gradiente[n][q][1] /= mgrilla.dy;

				vector_gradiente_calculado[n] = true;
			}

			for( unsigned int j=1; j < mgrilla.coordenadas_y.size()-1 ; j++ )
			{
				for( unsigned int q=0; q < numero_de_densidades() ; q++ )
				{
					int n = fg_indice_de_voxel(i,j,k);
					vectores_gradiente[n][q][1] = (*p_vectores_densidad)[n+thomas_salto_en_j][q];
					vectores_gradiente[n][q][1] -= (*p_vectores_densidad)[n-thomas_salto_en_j][q];
					vectores_gradiente[n][q][1] /= dos_dy;

					vector_gradiente_calculado[n] = true;
				}
			}

		}
	}


	#pragma omp parallel for
	for( unsigned int j=0; j < mgrilla.coordenadas_y.size() ; j++ )
	{
		for( unsigned int i=0; i < mgrilla.coordenadas_x.size() ; i++ )
		{
			// endcaps
			for( unsigned int q=0; q < numero_de_densidades() ; q++ )
			{
				int k = 0;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][2] = (*p_vectores_densidad)[n+thomas_salto_en_k][q];
				vectores_gradiente[n][q][2] -= (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][2] /= mgrilla.dz;

				vector_gradiente_calculado[n] = true;
			}
			for( unsigned int q=0; q < numero_de_densidades() ; q++ )
			{
				int k = mgrilla.coordenadas_z.size()-1;
				int n = fg_indice_de_voxel(i,j,k);
				vectores_gradiente[n][q][2] = (*p_vectores_densidad)[n][q];
				vectores_gradiente[n][q][2] -= (*p_vectores_densidad)[n-thomas_salto_en_k][q];
				vectores_gradiente[n][q][2] /= mgrilla.dz;

				vector_gradiente_calculado[n] = true;
			}

			for( unsigned int k=1; k < mgrilla.coordenadas_z.size()-1 ; k++ )
			{
				for( unsigned int q=0; q < numero_de_densidades() ; q++ )
				{
					int n = fg_indice_de_voxel(i,j,k);
					vectores_gradiente[n][q][2] = (*p_vectores_densidad)[n+thomas_salto_en_k][q];
					vectores_gradiente[n][q][2] -= (*p_vectores_densidad)[n-thomas_salto_en_k][q];
					vectores_gradiente[n][q][2] /= dos_dz;

					vector_gradiente_calculado[n] = true;
				}
			}

		}
	}

	return;

}

void Microambiente::calcular_vector_de_gradiente( int n ){

	static double dos_dx = mgrilla.dx;
	static double dos_dy = mgrilla.dy;
	static double dos_dz = mgrilla.dz;
	static bool constantes_de_gradientes_definidas = false;
	std::vector<unsigned int> indices(3,0);

	if( constantes_de_gradientes_definidas == false )
	{
		dos_dx *= 2.0;
		dos_dy *= 2.0;
		dos_dz *= 2.0;
		constantes_de_gradientes_definidas = true;
	}


	indices = indices_cartesianos( n );

	if( indices[0] > 0 && indices[0] < mgrilla.coordenadas_x.size()-1 )
	{
		for( unsigned int q=0; q < numero_de_densidades() ; q++ )
		{
			vectores_gradiente[n][q][0] = (*p_vectores_densidad)[n+thomas_salto_en_i][q];
			vectores_gradiente[n][q][0] -= (*p_vectores_densidad)[n-thomas_salto_en_i][q];
			vectores_gradiente[n][q][0] /= dos_dx;

			vector_gradiente_calculado[n] = true;
		}
	}


	if( indices[1] > 0 && indices[1] < mgrilla.coordenadas_y.size()-1 )
	{
		for( unsigned int q=0; q < numero_de_densidades() ; q++ )
		{
			vectores_gradiente[n][q][1] = (*p_vectores_densidad)[n+thomas_salto_en_j][q];
			vectores_gradiente[n][q][1] -= (*p_vectores_densidad)[n-thomas_salto_en_j][q];
			vectores_gradiente[n][q][1] /= dos_dy;

			vector_gradiente_calculado[n] = true;
		}
	}


	if( indices[2] > 0 && indices[2] < mgrilla.coordenadas_z.size()-1 )
	{
		for( unsigned int q=0; q < numero_de_densidades() ; q++ )
		{
			vectores_gradiente[n][q][2] = (*p_vectores_densidad)[n+thomas_salto_en_k][q];
			vectores_gradiente[n][q][2] -= (*p_vectores_densidad)[n-thomas_salto_en_k][q];
			vectores_gradiente[n][q][2] /= dos_dz;

			vector_gradiente_calculado[n] = true;
		}
	}

	return;


}

void Microambiente::resetear_todos_los_vectores_de_gradientes( void ){

	for( unsigned int k=0 ; k < mgrilla.voxeles.size() ; k++ )
	{
		for( unsigned int i=0 ; i < numero_de_densidades() ; i++ )
		{
			(vectores_gradiente[k])[i].resize( 3, 0.0 );
		}
	}
	vector_gradiente_calculado.assign( mgrilla.voxeles.size() , false );


}

/////////////////////////////////////////////////////////////


void Microambiente::simular_difusion_decaimiento( double dt ){

	solver_decaimiento_de_la_difusion__coeficientes_constantes_LOD_3D( dt );

	return;

}



void Microambiente::agregar_nodo_de_dirichlet( int indice_de_voxel, std::vector<double>& valor ){

	mgrilla.voxeles[indice_de_voxel].es_dirichlet=true;

	vector_valores_de_dirichlet[indice_de_voxel] = valor; 

	return;

}

void Microambiente::actualizar_nodo_de_dirichlet( int indice_de_voxel , std::vector<double>& nuevo_valor ){

	mgrilla.voxeles[indice_de_voxel].es_dirichlet = true;
	vector_valores_de_dirichlet[indice_de_voxel] = nuevo_valor;

	return;
}

void Microambiente::actualizar_nodo_de_dirichlet( int indice_de_voxel , int indice_de_sustrato, double nuevo_valor){

	mgrilla.voxeles[indice_de_voxel].es_dirichlet = true;
	vector_valores_de_dirichlet[indice_de_voxel][indice_de_sustrato] = nuevo_valor;

	dirichlet_vectorES_activacion[indice_de_voxel][indice_de_sustrato] = true;

	return;
}

void Microambiente::aplicar_condiciones_de_dirichlet( void ){

	#pragma omp parallel for
	for( unsigned int i=0 ; i < mgrilla.voxeles.size() ;i++ )
	{
		if( mgrilla.voxeles[i].es_dirichlet == true )
		{
			for( unsigned int j=0; j < vector_valores_de_dirichlet[i].size(); j++ )
			{
				if( dirichlet_vectorES_activacion[i][j] == true )
				{
					vector_de_densidades(i)[j] = vector_valores_de_dirichlet[i][j]; 
				}
			}

		}
	}
	return;
}

void Microambiente::set_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato , bool nuevo_valor ){

	vector_activacion_dirichlet[indice_de_sustrato] = nuevo_valor;

	for( unsigned int n = 0 ; n < mgrilla.voxeles.size() ; n++ )
	{ dirichlet_vectorES_activacion[n][indice_de_sustrato] = nuevo_valor; }

	return;
}

void Microambiente::set_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato , int indice, bool nuevo_valor ){

	dirichlet_vectorES_activacion[indice][indice_de_sustrato] = nuevo_valor;
	return;

}

void Microambiente::set_activacion_de_sustrato_de_dirichlet( int indice, std::vector<bool>& nuevo_valor ){

	dirichlet_vectorES_activacion[indice] = nuevo_valor;
	return;

}

bool Microambiente::get_activacion_de_sustrato_de_dirichlet( int indice_de_sustrato, int indice ){

	return vector_activacion_dirichlet[indice_de_sustrato];

}

bool& Microambiente::es_nodo_de_dirichlet( int indice_de_voxel ){

	return mgrilla.voxeles[indice_de_voxel].es_dirichlet;
}

void Microambiente::solver_decaimiento_de_la_difusion__coeficientes_constantes_LOD_3D( double dt ){

	//std::cout << "Solver hecho? \n";
	if( !setup_del_solver_de_difusion_hecho )
	{
	//	std::cout << "No \n";

		thomas_denomx.resize( mgrilla.coordenadas_x.size() , cero );
		thomas_cx.resize( mgrilla.coordenadas_x.size() , cero );

		thomas_denomy.resize( mgrilla.coordenadas_y.size() , cero );
		thomas_cy.resize( mgrilla.coordenadas_y.size() , cero );

		thomas_denomz.resize( mgrilla.coordenadas_z.size() , cero);
		thomas_cz.resize( mgrilla.coordenadas_z.size() , cero );

		thomas_salto_en_i = 1;
		thomas_salto_en_j = mgrilla.coordenadas_x.size();
		thomas_salto_en_k = thomas_salto_en_j * mgrilla.coordenadas_y.size();

		thomas_constante1 =  coeficientes_de_difusion; 
		thomas_constante1a = cero; 
		thomas_constante2 =  tasas_de_decaimiento; 
		thomas_constante3 = uno; 
		thomas_constante3a = uno; 

		thomas_constante1 *= dt;
		thomas_constante1 /= mgrilla.dx;
		thomas_constante1 /= mgrilla.dx;

		thomas_constante1a = thomas_constante1;
		thomas_constante1a *= -1.0;

		thomas_constante2 *= dt;
		thomas_constante2 /= 3.0; 

		thomas_constante3 += thomas_constante1;
		thomas_constante3 += thomas_constante1;
		thomas_constante3 += thomas_constante2;

		thomas_constante3a += thomas_constante1;
		thomas_constante3a += thomas_constante2;

		thomas_cx.assign( mgrilla.coordenadas_x.size() , thomas_constante1a );
		thomas_denomx.assign( mgrilla.coordenadas_x.size()  , thomas_constante3 );
		thomas_denomx[0] = thomas_constante3a;
		thomas_denomx[ mgrilla.coordenadas_x.size()-1 ] = thomas_constante3a;
		if( mgrilla.coordenadas_x.size() == 1 )
		{ thomas_denomx[0] = uno; thomas_denomx[0] += thomas_constante2; }

		thomas_cx[0] /= thomas_denomx[0];
		for( unsigned int i=1 ; i <= mgrilla.coordenadas_x.size()-1 ; i++ )
		{
			axpy( &thomas_denomx[i] , thomas_constante1 , thomas_cx[i-1] );
			thomas_cx[i] /= thomas_denomx[i]; 
		}

		thomas_cy.assign( mgrilla.coordenadas_y.size() , thomas_constante1a );
		thomas_denomy.assign( mgrilla.coordenadas_y.size()  , thomas_constante3 );
		thomas_denomy[0] = thomas_constante3a;
		thomas_denomy[ mgrilla.coordenadas_y.size()-1 ] = thomas_constante3a;
		if( mgrilla.coordenadas_y.size() == 1 )
		{thomas_denomy[0] = uno; thomas_denomy[0] += thomas_constante2; }

		thomas_cy[0] /= thomas_denomy[0];
		for( unsigned int i=1 ; i <= mgrilla.coordenadas_y.size()-1 ; i++ )
		{
			axpy( &thomas_denomy[i] , thomas_constante1 , thomas_cy[i-1] );
			thomas_cy[i] /= thomas_denomy[i]; 
		}

		thomas_cz.assign( mgrilla.coordenadas_z.size() , thomas_constante1a );
		thomas_denomz.assign( mgrilla.coordenadas_z.size()  , thomas_constante3 );
		thomas_denomz[0] = thomas_constante3a;
		thomas_denomz[ mgrilla.coordenadas_z.size()-1 ] = thomas_constante3a;
		if( mgrilla.coordenadas_z.size() == 1 )
		{ thomas_denomz[0] = uno; thomas_denomz[0] += thomas_constante2; }

		thomas_cz[0] /= thomas_denomz[0];
		for( unsigned int i=1 ; i <= mgrilla.coordenadas_z.size()-1 ; i++ )
		{
			axpy( &thomas_denomz[i] , thomas_constante1 , thomas_cz[i-1] );
			thomas_cz[i] /= thomas_denomz[i]; 
		}

		setup_del_solver_de_difusion_hecho = true;
	//	std::cout << "Ahora si \n";
	}

	//std::cout << "Directamente si \n";


	aplicar_condiciones_de_dirichlet();

	#pragma omp parallel for
	for( unsigned int k=0; k < mgrilla.coordenadas_z.size() ; k++ )
	{
		for( unsigned int j=0; j < mgrilla.coordenadas_y.size() ; j++ )
		{

			int n = fg_indice_de_voxel(0,j,k);
			(*p_vectores_densidad)[n] /= thomas_denomx[0];

			for( unsigned int i=1; i < mgrilla.coordenadas_x.size() ; i++ )
			{
				n = fg_indice_de_voxel(i,j,k);
                for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
                    (*p_vectores_densidad)[n][m] += thomas_constante1[m] * (*p_vectores_densidad)[n-thomas_salto_en_i][m];
                }
				(*p_vectores_densidad)[n] /= thomas_denomx[i];
			}

			for( int i = mgrilla.coordenadas_x.size()-2 ; i >= 0 ; i-- )
			{
				n = fg_indice_de_voxel(i,j,k);
                for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
                    (*p_vectores_densidad)[n][m] -= thomas_cx[i][m] * (*p_vectores_densidad)[n+thomas_salto_en_i][m];
                }                
			}

		}
	}


	aplicar_condiciones_de_dirichlet();

	#pragma omp parallel for
	for( unsigned int k=0; k < mgrilla.coordenadas_z.size() ; k++ )
	{
		for( unsigned int i=0; i < mgrilla.coordenadas_x.size() ; i++ )
		{
	int n = fg_indice_de_voxel(i,0,k);
	(*p_vectores_densidad)[n] /= thomas_denomy[0];

	for( unsigned int j=1; j < mgrilla.coordenadas_y.size() ; j++ )
	{
		n = fg_indice_de_voxel(i,j,k);
       for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
            (*p_vectores_densidad)[n][m] += thomas_constante1[m] * (*p_vectores_densidad)[n-thomas_salto_en_j][m];
        }        
		(*p_vectores_densidad)[n] /= thomas_denomy[j];
	}


	for( int j = mgrilla.coordenadas_y.size()-2 ; j >= 0 ; j-- )
	{
		n = fg_indice_de_voxel(i,j,k);
		//naxpy( &(*p_vectores_densidad)[n] , thomas_cy[j] , (*p_vectores_densidad)[n+thomas_salto_en_j] );
        for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
            (*p_vectores_densidad)[n][m] -= thomas_cy[j][m] * (*p_vectores_densidad)[n+thomas_salto_en_j][m];
        }        
	}

  }
 }


	aplicar_condiciones_de_dirichlet();

 #pragma omp parallel for
 for( unsigned int j=0; j < mgrilla.coordenadas_y.size() ; j++ )
 {

  for( unsigned int i=0; i < mgrilla.coordenadas_x.size() ; i++ )
  {

	int n = fg_indice_de_voxel(i,j,0);
	(*p_vectores_densidad)[n] /= thomas_denomz[0];

	for( unsigned int k=1; k < mgrilla.coordenadas_z.size() ; k++ )
	{
		n = fg_indice_de_voxel(i,j,k);
       for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
            (*p_vectores_densidad)[n][m] += thomas_constante1[m] * (*p_vectores_densidad)[n-thomas_salto_en_k][m];
        }           
		(*p_vectores_densidad)[n] /= thomas_denomz[k];
	}

	for( int k = mgrilla.coordenadas_z.size()-2 ; k >= 0 ; k-- )
	{
		n = fg_indice_de_voxel(i,j,k);
        for( unsigned int m=0; m < (*p_vectores_densidad)[n].size() ; m++ ){
            (*p_vectores_densidad)[n][m] -= thomas_cz[k][m] * (*p_vectores_densidad)[n+thomas_salto_en_k][m];
        }        
	}
  }
 }

	aplicar_condiciones_de_dirichlet();


	return;
}


void Microambiente::mostrar_informacion( std::ostream& os )
{
	os << std::endl << "Resumen del Microambiente: " << nombre << ": " << std::endl;
	mgrilla.mostrar_informacion_cartesiano( os );
	os << "Densidades: (" << numero_de_densidades() << " en total)" << std::endl;
	for( unsigned int i = 0 ; i < densidades_nombres.size() ; i++ )
	{
		os << "   " << densidades_nombres[i] << ":" << std::endl
		<< "     unidades: " << densidades_unidades[i] << std::endl
		<< "     coeficiente de difusion: " << coeficientes_de_difusion[i]
			<< " " << unidades_espaciales << "^2 / " << unidades_temporales << std::endl
		<< "     tasa de decaimiento: " << tasas_de_decaimiento[i]
			<< " " << unidades_temporales << "^-1" << std::endl
		<< "     longitud de escala de la difusion: " << sqrt( coeficientes_de_difusion[i] / ( 1e-12 + tasas_de_decaimiento[i] ) )
			<< " " << unidades_espaciales << std::endl
		<< "     condicion inicial: " << pg->vector_condiciones_iniciales[i]
			<< " " << densidades_unidades[i] << std::endl
		<< "     condiciones de borde: " << pg->vector_condicion_de_dirichlet[i]
			<< " " << densidades_unidades[i] << " (activo: ";
		if( vector_activacion_dirichlet[i] == true )
		{ os << "si"; }
		else
		{ os << "no"; }
		os << ")" << std::endl;
	}
	os << std::endl;

	return;
}




void Microambiente::inicializar_microambiente(){

	nombre = pg->m_nombre;

    if( pg->usar_oxigeno_como_primer_sustrato == true )
	{
		set_densidad(0, "oxigeno" , "mmHg");
		coeficientes_de_difusion[0] = 1e5;
		tasas_de_decaimiento[0] = 0.1;
	}

/*	if( pg->activar_respuesta_inmune == true )
	{
		agregar_densidad("immunostimulatory factor" , "dimensionless", 1000, .016);
		vector_activacion_dirichlet[vector_activacion_dirichlet.size()] = false;
		pg->vector_condiciones_iniciales[pg->vector_condiciones_iniciales.size()] = 0.0;
	}
*/	
	if( pg->activar_sustancia_x == true )
	{
		agregar_densidad("sustancia x" , "dimensionless", 1000000, 0.5);
		vector_activacion_dirichlet[1] = false;
		pg->vector_condiciones_iniciales[1] = 1.0;
	}

	redimensionar_espacio( pg->rango_en_X[0], pg->rango_en_X[1] ,
		pg->rango_en_Y[0], pg->rango_en_Y[1],
		pg->rango_en_Z[0], pg->rango_en_Z[1],
		pg->m_dx,pg->m_dy,pg->m_dz );

	unidades_espaciales = pg->unidades_espaciales;
	unidades_temporales = pg->unidades_temporales;
	mgrilla.unidades = pg->unidades_espaciales;

	if( pg->vector_condiciones_iniciales.size() !=
		numero_de_densidades() )
	{
		pg->vector_condiciones_iniciales = pg->vector_condicion_de_dirichlet;
	}

	for( unsigned int n=0; n < numero_de_voxeles() ; n++ )
	{ vector_de_densidades(n) = pg->vector_condiciones_iniciales; }


 
	pg->dirichlet_xmin_valores = pg->vector_condicion_de_dirichlet;
	pg->dirichlet_xmax_valores = pg->vector_condicion_de_dirichlet;
	pg->dirichlet_ymin_valores = pg->vector_condicion_de_dirichlet;
	pg->dirichlet_ymax_valores = pg->vector_condicion_de_dirichlet;
	pg->dirichlet_zmin_valores = pg->vector_condicion_de_dirichlet;
	pg->dirichlet_zmax_valores = pg->vector_condicion_de_dirichlet;


	bool xmin = false;
	bool xmax = false;
	bool ymin = false;
	bool ymax = false;
	bool zmin = false;
	bool zmax = false;

	if( pg->condiciones_de_Dirichlet_externas == true )
	{
		for( unsigned int n=0 ; n < numero_de_densidades() ; n++ )
		{
			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_xmin[n] )
				{ xmin = true; }

			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_xmax[n] )
				{ xmax = true; }

			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_ymin[n] )
				{ ymin = true; }

			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_ymax[n] )
				{ ymax = true; }

			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_zmin[n] )
				{ zmin = true; }

			if( pg->dirichlet_todo[n] ||
				pg->dirichlet_zmax[n] )
				{ zmax = true; }
		}


	}

	if( pg->condiciones_de_Dirichlet_externas == true )
	{
		if( xmin == true )
		{
			for( unsigned int k=0 ; k < mgrilla.coordenadas_z.size() ; k++ )
			{
				int I = 0;
				for( unsigned int j=0 ; j < mgrilla.coordenadas_y.size() ; j++ )
				{
					// set the value
					agregar_nodo_de_dirichlet( fg_indice_de_voxel(I,j,k) , pg->dirichlet_xmin_valores );

					// set the activation
					set_activacion_de_sustrato_de_dirichlet( fg_indice_de_voxel(I,j,k) ,
					pg->dirichlet_xmin );

				}
			}
		}

		if( xmax == true )
		{
			for( unsigned int k=0 ; k < mgrilla.coordenadas_z.size() ; k++ )
			{
				int I = mgrilla.coordenadas_x.size()-1;;
				for( unsigned int j=0 ; j < mgrilla.coordenadas_y.size() ; j++ )
				{
					agregar_nodo_de_dirichlet( fg_indice_de_voxel(I,j,k) , pg->dirichlet_xmax_valores );

					set_activacion_de_sustrato_de_dirichlet( fg_indice_de_voxel(I,j,k) ,
					pg->dirichlet_xmax );
				}
			}
		}

		if( ymin == true )
		{
			for( unsigned int k=0 ; k < mgrilla.coordenadas_z.size() ; k++ )
			{
				int J = 0; 
				
				for( unsigned int i=0 ; i < mgrilla.coordenadas_x.size() ; i++ )
				{
					
					agregar_nodo_de_dirichlet( fg_indice_de_voxel(i,J,k) , pg->dirichlet_ymin_valores );

					
					set_activacion_de_sustrato_de_dirichlet( fg_indice_de_voxel(i,J,k) ,
					pg->dirichlet_ymin );
				}
			}
		}

		if( ymax == true )
		{
			for( unsigned int k=0 ; k < mgrilla.coordenadas_z.size() ; k++ )
			{
				int J = mgrilla.coordenadas_y.size()-1;;
				for( unsigned int i=0 ; i < mgrilla.coordenadas_x.size() ; i++ )
				{
					agregar_nodo_de_dirichlet( fg_indice_de_voxel(i,J,k) , pg->dirichlet_ymax_valores );

					set_activacion_de_sustrato_de_dirichlet(fg_indice_de_voxel(i,J,k) ,
					pg->dirichlet_ymax );
				}
			}
		}

			if( zmin == true )
			{
				for( unsigned int j=0 ; j < mgrilla.coordenadas_y.size() ; j++ )
				{
					int K = 0; 
					for( unsigned int i=0 ; i < mgrilla.coordenadas_x.size() ; i++ )
					{
						
						agregar_nodo_de_dirichlet( fg_indice_de_voxel(i,j,K) , pg->dirichlet_zmin_valores );

						
						set_activacion_de_sustrato_de_dirichlet( fg_indice_de_voxel(i,j,K),
						pg->dirichlet_zmin );
					}
				}
			}

			
			if( zmax == true )
			{
				for( unsigned int j=0 ; j < mgrilla.coordenadas_y.size() ; j++ )
				{
					int K = mgrilla.coordenadas_z.size()-1;;
					
					for( unsigned int i=0 ; i < mgrilla.coordenadas_x.size() ; i++ )
					{
						
						agregar_nodo_de_dirichlet( fg_indice_de_voxel(i,j,K) , pg->dirichlet_zmax_valores );

						
						set_activacion_de_sustrato_de_dirichlet( fg_indice_de_voxel(i,j,K) ,
						pg->dirichlet_zmax );
					}
				}
			}
		}


	for( unsigned int i=0 ; i < pg->vector_activacion_dirichlet.size(); i++ )
	{
		set_activacion_de_sustrato_de_dirichlet( i , pg->vector_activacion_dirichlet[i] );
	}

	mostrar_informacion( std::cout );
	return;
}



void Microambiente::crear_vaso_sanguineo( int xmin, int ymin, int zmin, int xmax, int ymax, int zmax ){


    int x1 = xmax, y1 = ymax, z1 = zmax, x0 = xmin, y0 = ymin, z0 = zmin;

    int dx = std::max(abs(x1 - x0), 1);
    int dy = std::max(abs(y1 - y0), 1);
    int dz = std::max(abs(z1 - z0), 1); 
    //std::cout << "dx: " << dx << " dy: " << dy << " dz: " << dz << "\n";

//    int stepX;
//    if (x0 < x1){
//        stepX = 1;
//    }else if (x0 > x1){
//        stepX = -1;
//    }else{
//        stepX = 0;
//    }
    int stepX = x0 < x1 ? 1 : -1;

//    int stepY;
//    if (y0 < y1){
//        stepY = 1;
//    }else if (y0 > y1){
//        stepY = -1;
//    }else{
//        stepY = 0;
//    }
    int stepY = y0 < y1 ? 1 : -1;

//    int stepZ;
//    if (z0 < z1){
//        stepZ = 1;
//    }else if (z0 > z1){
//        stepZ = -1;
//    }else{
//        stepZ = 0;
//    }
    int stepZ = z0 < z1 ? 1 : -1;

    //std::cout << "stepX: " << stepX << " stepY: " << stepY << " stepZ: " << stepZ << "\n";

    double hipotenusa = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    //std::cout << "hipotenusa: " << hipotenusa << "\n";

    double tMaxX = hipotenusa*0.5 / dx;
    double tMaxY = hipotenusa*0.5 / dy;
    double tMaxZ = hipotenusa*0.5 / dz;
    //std::cout << "tMaxX: " << tMaxX << " tMaxY: " << tMaxY << " tMaxZ: " << tMaxZ << "\n";

    double tDeltaX = hipotenusa / dx;
    double tDeltaY = hipotenusa / dy;
    double tDeltaZ = hipotenusa / dz;
    //std::cout << "tDeltaX: " << tDeltaX << " tDeltaY: " << tDeltaY << " tDeltaZ: " << tDeltaZ << "\n";

    int voxel_actual = -99;

    while (x0 != x1 || y0 != y1 || z0 != z1){
        if(tMaxX < tMaxY){
            if(tMaxX < tMaxZ){
                x0 = x0 + stepX;
                tMaxX = tMaxX + tDeltaX;
            }else if(tMaxX < tMaxZ){
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }else{
                x0 = x0 + stepX;
                tMaxX = tMaxX + tDeltaX;
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }
        }else if(tMaxX > tMaxY){
            if(tMaxY < tMaxZ){
                y0 = y0 + stepY;
                tMaxY = tMaxY + tDeltaY;
            }else if(tMaxY > tMaxZ){
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }else{
                y0 = y0 + stepY;
                tMaxY = tMaxY + tDeltaY;
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }
        }else{
            if(tMaxY < tMaxZ){
                y0 = y0 + stepY;
                tMaxY = tMaxY + tDeltaY;
                x0 = x0 + stepX;
                tMaxX = tMaxX + tDeltaX;
            }else if(tMaxY > tMaxZ){
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }else{
                x0 = x0 + stepX;
                tMaxX = tMaxX + tDeltaX;
                y0 = y0 + stepY;
                tMaxY = tMaxY + tDeltaY;
                z0 = z0 + stepZ;
                tMaxZ = tMaxZ + tDeltaZ;
            }
        }

        Vector posicion;
        posicion.x=x0;
        posicion.y=y0;
        posicion.z=z0;

        int voxel_siguiente = indice_del_voxel_mas_cercano(posicion);
        //std::cout << "Indice del voxel mas cercano: " <<  m->indice_del_voxel_mas_cercano(posicion) << "\n";
        //std::cout << "Pongo el voxel en un vector para controlar \n";

        if(voxel_actual != voxel_siguiente){
            //Pongo el voxel en un vector para controlar
            voxeles_del_vaso_sanguineo.push_back(indice_del_voxel_mas_cercano(posicion));
            //std::cout << "x0: " << x0 << " y0: " << y0 << " z0: " << z0 << "\n";

            //std::cout << "seteo el valor \n";
            //seteo el valor
            agregar_nodo_de_dirichlet(indice_del_voxel_mas_cercano(posicion), pg->vector_condicion_de_dirichlet);

            //std::cout << "seteo la activacion \n";
            //seteo la activacion
            set_activacion_de_sustrato_de_dirichlet(indice_del_voxel_mas_cercano(posicion), pg->dirichlet_vs);

            voxel_actual = voxel_siguiente;
        }
    }

//    for(long unsigned int j=0; j<voxeles_del_vaso_sanguineo.size(); j++){
//
//        std::cout << "Voxel: " << voxeles_del_vaso_sanguineo[j] << "\n";
//
//    };
//	std::cout << "pulse enter para continuar \n";
//
//    std::cin.get();
    //voxeles_del_vaso_sanguineo.clear();
////////////////////////////////////////////////////////////////
//	for( int k=zmin ; k < zmax ; k++ )
//	{
//		 //set Dirichlet conditions along the xmin outer edges
//		for( int j=ymin ; j < ymax ; j++ )
//		{
//            for( int i=xmin ; i < xmax ; i++ )
//            {
//
//                 //set the value
//                m->agregar_nodo_de_dirichlet( m->indice_de_voxel(i,j,k) , pg->vector_condicion_de_dirichlet );
//
//                 s//et the activation
//                m->set_activacion_de_sustrato_de_dirichlet( m->indice_de_voxel(i,j,k), true );
//
//            }
//        }
//    }

    return;
}


void Microambiente::posibles_nodos(std::vector<Vector>& vector_posiciones){
 
    int indice;
    
    for(int j=0; j< vector_posiciones.size(); j++){
        
        for(int i=0; i < mgrilla.coordenadas_z.size(); i++){
            
            indice = indice_de_voxel(vector_posiciones[j].x, vector_posiciones[j].y, i);
            voxeles(indice).posible_nodo=true;
            
            
            posibles_voxeles_del_vaso_sanguineo.push_back(indice);
            
        }        
    }
}

void Microambiente::vasos_del_higado(double radio, std::vector<Vector>& indices){
    
    
    for(int j=0; j<indices.size(); j++){
        
        int indice = (indices[j].z*mgrilla.coordenadas_y.size() + indices[j].y )*mgrilla.coordenadas_x.size() + indices[j].x;
        
 /*       std::cout << voxeles(indice).centro.x << " " << voxeles(indice).centro.y << "\n";
        
        std::cout << radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y << "\n";
        
       std::cout << "Z= " << sqrt(radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y) << " Z cuadrado = " << radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y << "\n";
        
        std::cin.get();
 */       
        if(radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y > 0 ){
            
//             std::cout << "ENTRE AL IF \n";       
            
            crear_vaso_sanguineo( voxeles(indice).centro.x, voxeles(indice).centro.y, 
                                  (int)-sqrt(radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y), 
                                  voxeles(indice).centro.x+1, voxeles(indice).centro.y+1,
                                  (int)sqrt(radio*radio - voxeles(indice).centro.x*voxeles(indice).centro.x - voxeles(indice).centro.y*voxeles(indice).centro.y));
            
        }
    }
    //std::cin.get();
}



