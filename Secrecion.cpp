#include "Secrecion.h"

Secrecion::Secrecion()
{
	tasas_de_secrecion.resize( 0 , 0.0 );
	tasas_de_consumo.resize( 0 , 0.0 );
	densidades_de_saturacion.resize( 0 , 0.0 );
	tasas_de_exportacion_neta.resize( 0 , 0.0 );
	oncoproteina = 0.0;
	return;
}




void Secrecion::set_todas_las_secreciones_a_cero( void )
{
	for( unsigned int i=0; i < tasas_de_secrecion.size(); i++ )
	{
		tasas_de_secrecion[i] = 0.0;
		tasas_de_exportacion_neta[i] = 0.0;
	}
	return;
}


void Secrecion::set_todos_los_consumos_a_cero( void )
{
	for( unsigned int i=0; i < tasas_de_consumo.size(); i++ )
	{ tasas_de_consumo[i] = 0.0; }
	return;
}


void Secrecion::multiplicar_las_secreciones_por_un_factor( double factor )
{
	for( unsigned int i=0; i < tasas_de_secrecion.size(); i++ )
	{
		tasas_de_secrecion[i] *= factor;
		tasas_de_exportacion_neta[i] *= factor;
	}
	return;
}


void Secrecion::multiplicar_los_consumos_por_un_factor( double factor )
{
	for( unsigned int i=0; i < tasas_de_consumo.size(); i++ )
	{ tasas_de_consumo[i] *= factor; }
	return;
}

