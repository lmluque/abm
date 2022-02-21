#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <fstream>
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include<iomanip>
#include <chrono>
#include <ctime>

#include "Macros.h"
#include "Tejido.h"



RNG *rng;
Vector *v;
Constantes *c;
Ciclo_Modelo Ki67, vida, hepato_30, hepato_70, ki67_cancer, necrosis, apoptosis;
Muerte_parametros necrosis_parametros, apoptosis_parametros;

Parametros_globales parametros_globales;
Parametros_globales *pg = &parametros_globales;



using namespace std;



std::vector<Celula*> todas_las_celulas;
std::vector<Celula*> celulas_listas_para_dividirse;
std::vector<Celula*> celulas_listas_para_remover;
std::vector<Celula*> celulas_para_registrar_en_voxeles;
bool ciclo_celular_estandar_inicializado;
bool ciclo_celular_de_muerte_inicializado;
std::vector<double> datos_finales_T;
std::vector<double> datos_finales_V;

int main(int argc, char *argv[]){
    std::string inputFile(argv[1]);
    std::string parametro;
	std::string valor;
//	cout << "leyendo parametros desde " << inputFile << "\n";
//	cout << "pulse enter para continuar \n";
	//cin.get();
	ifstream param(inputFile);
	if (!param.is_open()) {
		cout << "ERROR: No se puede abrir el archivo " << inputFile << endl;
		exit(1);
	}
	while (!param.eof()) {
		param >> parametro;
		param >> valor;
		cout << parametro << " = " << valor << endl;
		pg->set_parametros(parametro, valor);
	}


	rng = new RNG(1.0, pg->seed, pg->imm_mean, pg->imm_sd);
	pg->tiempo_total=0.0;
	pg->tiempo_final = 86400.01;
    double tiempo_de_escritura = 60.0;



	Tejido tejido;
	crear_ciclo_celular_estandar();
	crear_ciclo_de_muerte_estandar();


	pg->ciclo = Ki67;
	pg->numero_id = 0;
    pg->hepatectomia = false;



	tejido.inicializar_tejido();
    
//    for(int j=0; j<20; j++){
//        std::cout << rng->RandomNumber(pg->rango_en_X[0],pg->rango_en_X[1]) << "\n";
//        std::cin.get();
//    }
    
//    cout << "Cantidad de celulas: " << todas_las_celulas.size() << "\n";

//	tejido.microambiente.mostrar_informacion(cout);
//	cout << "pulse enter para continuar \n";
//	tejido.cdc.grillado.mostrar_informacion_cartesiano(cout);
//	cout << "pulse enter para continuar \n";
//	tejido.cdc.celula->mostrar_informacion_de_la_celula(cout);
//	tejido.cdc.celula->fenotipo.ciclo.pCiclo_Modelo->mostrar_ciclo(cout);
//	cout << "pulse enter para continuar \n";


//	cout << "TASAS DE TRANSICION \n";
//	cout << tejido.cdc.celula->fenotipo.ciclo.tasas_de_transicion[0][0] << "\n";
//	cout << tejido.cdc.celula->fenotipo.ciclo.tasas_de_transicion[1][0] << "\n";
//	cout << tejido.cdc.celula->fenotipo.ciclo.tasas_de_transicion[2][0] << "\n";
//	cout << "pulse enter para continuar \n";
//	cin.get();

    /////////////////CHEQUEAR VASO SANGUINEO CON CELULAS////////////////////
/*
	tejido.microambiente.crear_vaso_sanguineo(pg->rango_en_X[0],pg->rango_en_Y[0],pg->rango_en_Z[0],pg->rango_en_X[0]+1,pg->rango_en_Y[0]+1,pg->rango_en_Z[0]+1);

	std::cout << "cantidad de voxeles: " << tejido.microambiente.voxeles_del_vaso_sanguineo.size() << "\n";
    //std::cin.get();
*/

////////////////////////////////////////////////////////////////////////
    tejido.geometria_del_tumor();
    std::ofstream outfile("DatosFinales.dat", ios::app);
    outfile << "#tiempo, volumen, volumen2, radio , celulas tumorales, celulas muertas, todas las celulas, celulas divididas \n";
    outfile << std::setprecision(12) << pg->tiempo_total << " " << tejido.volumen_del_tumor << " " << tejido.volumen_del_tumor2 << " " << tejido.radio_del_tumor << " " << tejido.celulas_tumorales << " " << tejido.celulas_muertas << " " << todas_las_celulas.size() << " " << celulas_listas_para_dividirse.size() << "\n";
    outfile.flush();
    //auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
    //std::cout << ctime(&timenow) << pg->tiempo_total << " " << tejido.volumen_del_tumor << "\n";


    double n = 0;
    double m = 0;
    int itr = 0;
    static double tolerancia_de_la_mecanica = 0.001 * c->dt_mecanica;   
    static double tolerancia_de_la_escritura = 0.001 * c->dt_ciclo;
    
    for (pg->tiempo_total =0; pg->tiempo_total <= pg->tiempo_final; pg->tiempo_total+=c->dt_difusion){
        //cout << "flag1 \n";
/*
            static bool introduje_celulas_inmunes = false;
			if( pg->activar_respuesta_inmune == true && pg->tiempo_total > pg->tiempo_de_imm - 0.01*c->dt_difusion && introduje_celulas_inmunes == false ){
            	tejido.introducir_linfocitos_aleatorios(pg->cantidad_de_linfocitos);
            	introduje_celulas_inmunes = true;
        }

            static bool introduje_celulas_inmunes2 = false;
			if( pg->activar_respuesta_inmune == true && pg->tiempo_total > pg->tiempo_de_imm_2 - 0.01*c->dt_difusion && introduje_celulas_inmunes2 == false ){
            	tejido.introducir_linfocitos_aleatorios(pg->cantidad_de_linfocitos);
            	introduje_celulas_inmunes2 = true;
        }
*/

            //static bool hepatectomia = false;
			if( pg->tiempo_total > 200.0 - 0.01*c->dt_difusion && pg->hepatectomia == false ){
            	tejido.cortar_higado(70);
            	pg->hepatectomia = true;
        }
        
//cout << "flag2 \n";
		tejido.microambiente.simular_difusion_decaimiento( c->dt_difusion );
//cout << "flag3 \n";
        double T_mec = pg->tiempo_total - n;
        if( pg->calcular_gradientes && fabs(T_mec - c->dt_mecanica) < tolerancia_de_la_mecanica){
                tejido.microambiente.calcular_todos_los_vectores_de_gradientes();
                n = pg->tiempo_total;
        }
//cout << "flag4 \n";
        tejido.cdc.actualizar_todas_las_celulas(pg->tiempo_total, c->dt_difusion, c->dt_mecanica, c->dt_ciclo);
//cout << "flag5 \n";

        double T_esc = pg->tiempo_total - m;
        if( fabs(T_esc - tiempo_de_escritura) < tolerancia_de_la_escritura){
            //cout << "flag5.1 \n";
                tejido.geometria_del_tumor();
                //auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
                //std::cout << ctime(&timenow) << pg->tiempo_total << " " << tejido.volumen_del_tumor << "\n";
                outfile << std::setprecision(12) << pg->tiempo_total << " " << tejido.volumen_del_tumor << " " << tejido.volumen_del_tumor2 << " " << tejido.radio_del_tumor << " " << tejido.celulas_tumorales << " " << tejido.celulas_muertas << " " << todas_las_celulas.size() << " " << celulas_listas_para_dividirse.size() << "\n";
                outfile.flush();
                m = pg->tiempo_total;
                //cout << "flag5.2 \n";
/////////////////////////////////////////////////////////////////////////
                
///*
				std::ofstream outfile2 ("Datos_" + std::to_string( (int) pg->tiempo_total ) + ".xyz" );
                
                //cout << "flag5.2.1 \n";
				outfile2 << todas_las_celulas.size() << "\n";
                //cout << "flag5.2.2 \n";
				outfile2 << "\n";
                //cout << "flag5.2.3 \n";
				int fallecida;
				string  nombreciclo;
                //cout << "flag5.3 \n";

				for( unsigned int i=0; i < todas_las_celulas.size(); i++ ){
                    
					nombreciclo = todas_las_celulas[i]->fenotipo.ciclo.pCiclo_Modelo->nombre;
                    outfile2 << todas_las_celulas[i]->id << " " << todas_las_celulas[i]->voxel << " " << todas_las_celulas[i]->posicion.x << " " << todas_las_celulas[i]->posicion.y << " " << todas_las_celulas[i]->posicion.z << " " << todas_las_celulas[i]->fenotipo.geometria.radio << " " << todas_las_celulas[i]->voxel_del_microambiente << " " << todas_las_celulas[i]->vector_de_densidades_mas_cercano()[0] << " " << todas_las_celulas[i]->vector_de_densidades_mas_cercano()[1] << " "<< todas_las_celulas[i]->fenotipo.secrecion.oncoproteina << " " << todas_las_celulas[i]->madre << " " << nombreciclo << " " << todas_las_celulas[i]->divisiones << " " << todas_las_celulas[i]->divisiones_permitidas << " " << todas_las_celulas[i]->probabilidad_aleat << " " << todas_las_celulas[i]->tipo << "\n";
                    

                }
// */
                
                std::ofstream outfile5 ("Datos70.dat", ios::app );
                    outfile5 << pg->tiempo_total << " " << todas_las_celulas[1]->fenotipo.volumen.total << " " << todas_las_celulas[1]->fenotipo.volumen.total/todas_las_celulas[1]->fenotipo.volumen.ideal << "\n";
                    outfile5.flush();
                                
                //cout << "flag5.4 \n";
                
                double promedio = 0.0;
                for(int j=0; j< tejido.microambiente.numero_de_voxeles() ; j++){
                    
                    promedio += tejido.microambiente.vector_de_densidades(j)[1];
                
                }
                //cout << "flag5.5 \n";
                
                promedio /= tejido.microambiente.numero_de_voxeles();
                
                int hemia=0;
                if(pg->hepatectomia){
                    hemia=1;
                }
                
                cout << "Tiempo total: " << pg->tiempo_total << "\n";
                std::ofstream outfile6 ("Hepato30.dat", ios::app);
                outfile6 << pg->tiempo_total << " " << todas_las_celulas[1]->id << " " << todas_las_celulas[1]->vector_de_densidades_mas_cercano()[1] << " " << promedio << "\n";
                outfile6.flush();
                //cout << "flag7 \n";
//-----------------VTKs----------------
                     std::ofstream outfile3 ("PM1_" + std::to_string( (int) pg->tiempo_total ) + ".xml" );
                    outfile3 << "<VTKFile type='UnstructuredGrid' version='0.1' byte_order='LittleEndian'> \n";
                    outfile3 << "\t<UnstructuredGrid>\n";
                    outfile3 << "\t\t<Piece NumberOfPoints='" << todas_las_celulas.size() << "' NumberOfCells='0'>\n";
                    outfile3 << "\t\t\t<Points>\n";
                    outfile3 << "\t\t\t\t<DataArray name='Position' type='Float32' NumberOfComponents='3' format='ascii'>\n";
                    for(long unsigned int j=0; j<todas_las_celulas.size(); j++){
                        outfile3 << "\t\t\t\t" << todas_las_celulas[j]->posicion.x << " " << todas_las_celulas[j]->posicion.y << " " << todas_las_celulas[j]->posicion.z << "\n";
                    }
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t</Points>\n";
                    outfile3 << "\t\t\t<PointData  Vectors='vector'>\n";
                    outfile3 << "\t\t\t\t<DataArray type='Float32' Name='Radio' format='ascii'>\n";
                    for(long unsigned int j=0; j<todas_las_celulas.size(); j++){
                        outfile3 << "\t\t\t\t" << todas_las_celulas[j]->fenotipo.geometria.radio << "\n";
                    }
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t\t<DataArray type='Float32' Name='Oncoproteina' format='ascii'>\n";
                    for(long unsigned int j=0; j<todas_las_celulas.size(); j++){
                        outfile3 << "\t\t\t\t" << todas_las_celulas[j]->fenotipo.secrecion.oncoproteina << "\n";
                    }
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t\t<DataArray type='Float32' Name='Tipo' format='ascii'>\n";

                    for(long unsigned int j=0; j<todas_las_celulas.size(); j++){
                    int tipo2 = todas_las_celulas[j]->tipo;
                    if(todas_las_celulas[j]->fenotipo.muerte.muerta == true){
                        tipo2=1;
                    }
                        outfile3 << "\t\t\t\t" << tipo2 << "\n";
                    }
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t</PointData>\n";
                    outfile3 << "\t\t\t<Cells>\n";
                    outfile3 << "\t\t\t\t<DataArray type='Int32' Name='connectivity' format='ascii'>\n";
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t\t<DataArray type='Int32' Name='offsets' format='ascii'>\n";
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t\t<DataArray type='UInt8' Name='types' format='ascii'>\n";
                    outfile3 << "\t\t\t\t</DataArray>\n";
                    outfile3 << "\t\t\t</Cells>\n";
                    outfile3 << "\t\t</Piece>\n";
                    outfile3 << "\t</UnstructuredGrid>\n";
                    outfile3 << "</VTKFile>";



                    std::ofstream outfile4("HM1_" + std::to_string( (int) pg->tiempo_total ) + ".xml" );
                    outfile4 << "<VTKFile type='UnstructuredGrid' version='0.1' byte_order='LittleEndian'> \n";
                    outfile4 << "\t<UnstructuredGrid>\n";
                    outfile4 << "\t\t<Piece NumberOfPoints='" << tejido.microambiente.mgrilla.voxeles.size() << "' NumberOfCells='0'>\n";
                    outfile4 << "\t\t\t<Points>\n";
                    outfile4 << "\t\t\t\t<DataArray name='Position' type='Float32' NumberOfComponents='3' format='ascii'>\n";
                    for(long unsigned int j=0; j<tejido.microambiente.mgrilla.voxeles.size(); j++){
                        outfile4 << "\t\t\t\t" << tejido.microambiente.mgrilla.voxeles[j].centro.x << " " << tejido.microambiente.mgrilla.voxeles[j].centro.y << " " << tejido.microambiente.mgrilla.voxeles[j].centro.z << "\n";
                    }
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t</Points>\n";
                    outfile4 << "\t\t\t<PointData  Vectors='vector'>\n";
                    outfile4 << "\t\t\t\t<DataArray type='Float32' Name='Radio' format='ascii'>\n";
                    for(long unsigned int j=0; j<tejido.microambiente.mgrilla.voxeles.size(); j++){
                        outfile4 << "\t\t\t\t" << pg->m_dx << "\n";
                    }
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t\t<DataArray type='Float32' Name='Oxigeno' format='ascii'>\n";
                    for(long unsigned int j=0; j<tejido.microambiente.mgrilla.voxeles.size(); j++){
                        outfile4 << "\t\t\t\t" << tejido.microambiente.vector_de_densidades(j)[0] << "\n";
                    }
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t\t<DataArray type='Float32' Name='Dirichlet' format='ascii'>\n";
                    for(long unsigned int j=0; j<tejido.microambiente.mgrilla.voxeles.size(); j++){
                        
                        int es_dirichlet = 0;
                        if(tejido.microambiente.mgrilla.voxeles[j].es_dirichlet==true){
                            es_dirichlet=1;
                        }
                        outfile4 << "\t\t\t\t" << es_dirichlet << "\n";
                    }
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t</PointData>\n";
                    outfile4 << "\t\t\t<Cells>\n";
                    outfile4 << "\t\t\t\t<DataArray type='Int32' Name='connectivity' format='ascii'>\n";
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t\t<DataArray type='Int32' Name='offsets' format='ascii'>\n";
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t\t<DataArray type='UInt8' Name='types' format='ascii'>\n";
                    outfile4 << "\t\t\t\t</DataArray>\n";
                    outfile4 << "\t\t\t</Cells>\n";
                    outfile4 << "\t\t</Piece>\n";
                    outfile4 << "\t</UnstructuredGrid>\n";
                    outfile4 << "</VTKFile>";

  
  
                
                
/////////////////////////////////////////////////////////////////////////                
                
                
        }

        if(fmod(pg->tiempo_total, c->dt_mecanica*20)<0.1){
			tejido.cdc.actualizar_voxeles_de_celulas();
		}
		
		if(pg->tiempo_total>1 && fmod(pg->tiempo_total, 360.0)<0.1){
			tejido.controlar_vasos_sanguineos();
		}
	}

	return 0;

}




/*
Para debuggear el código
#define INFO(msg) \
    fprintf(stderr, "info: %s:%d: ", __FILE__, __LINE__); \
    fprintf(stderr, "%s\n", msg);
*/
