#ifndef __MECANICA_H__
#define __MECANICA_H__

class Mecanica
{
 public:
    double fuerza_de_repulsion_cc;
    double fuerza_de_adhesion_cc;
    //Fuerzas con çélulas de otro tipo
    double fuerza_de_repulsion_co;
    double fuerza_de_adhesion_co;
    //Fuerzas con la membraba basal
    double fuerza_de_repulsion_mb;
    double fuerza_de_adhesion_mb;
	double distancia_de_adhesion_maxima_relativa;


	Mecanica(); // done




};

#endif
