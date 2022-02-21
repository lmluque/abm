#include "Voxel.h"

Voxel::Voxel()
{
	indice = 0; 
	volumen = 10*10*10;
//	centro; 
	es_dirichlet = false;
    posible_nodo = false;
}

std::ostream& operator<<(std::ostream& os, const Voxel& v)  
{
	static std::string tabbing = "\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t"; 
	os	<< tabbing << "<voxel ID=\"" << v.indice << "\">"  << std::endl
		<< tabbing2 << "centro: " << v.centro << std::endl  
		<< tabbing2 << "volumen: " << v.volumen << std::endl; 

 return os; 
}

void Voxel::stream_output_con_unidades( std::ostream& os , std::string unidades ) const 
{
	static std::string tabbing = "\t\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t\t"; 
	os	<< tabbing << "<voxel ID=\"" << indice << "\">"  << std::endl
		<< tabbing2 << "<centro " << centro << " unidades=\"" << unidades << "\" />" << std::endl 
		<< tabbing2 << "<volumen= " << volumen << " unidades= " << unidades << " cubicos\">" << std::endl		
//		<< tabbing2 << "<unidades de volumen=\ " << unidades << " cubicos\">" << volumen << "</volumen>" << std::endl
		<< tabbing  << "</voxel>"; 
	return; 
}
