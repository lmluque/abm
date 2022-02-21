#ifndef __VOXEL_H__
#define __VOXEL_H__

#include "Vector.h"
#include <iostream>

class Voxel
{

 private:
	friend std::ostream& operator<<(std::ostream& os, const Voxel& v); 
	/*!< outputs the Voxel to an open ostream 
	 * \param os -- the stream 
	 * \param mv -- the voxel you use this friendly friend operator on
	 * Example: Voxel v; 
	 *          cout << v << endl; 
	*/ 

 public:
	Voxel(); 
	int indice; /*!< voxel's index in a General_Mesh */ 

	double volumen; /*!< voxel's volume (cubic spatial units) */ 
	Vector centro; /*!< center of volume */
	bool es_dirichlet;
    bool posible_nodo;
	void stream_output_con_unidades( std::ostream& os , std::string unidades ) const;
};


#endif
