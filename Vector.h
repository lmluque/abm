/*! \file Vector.h
 * \brief Archivo de cabecera para la clase Vector
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1.0
 * \date 2020.06.12
 */

#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>
#include <vector>
#include <cmath>

class Vector {
private:

public:
	double x, y, z;
    /** Constructor
    * Inicializa el constructor con parámetros por defecto:
    * x = 0.0, y = 0.0, z = 0.0
    */
    Vector();

    /** Constructor
    * \param double x1 : componente x del vector.
    * \param double y1 : componente y del vector.
    * \param double z1 : componente z del vector
    */
    Vector(double x1, double y1, double z1);

    /** \fn operator+
    * \brief Sobrecarga el operador + para sumar dos
    * vectores.
    * \param Vector v : Vector que voy a sumar.
    * \return Vector
    */
    Vector operator+(Vector v);

    /** \fn operator-
    * \brief Sobrecarga el operador - para restar dos
    * vectores.
    * \param Vector v : Vector que voy a restar.
    * \return Vector
    */
    Vector operator-(Vector v);

    /** \fn operator*
    * \brief Sobrecarga el operador * para realizar el
    * producto escalar entre dos vectores.
    * \param Vector v : vector con el cual voy a realizar
    * el producto escalar.
    * \return double
    */
    double operator*(Vector v);

    /** \fn operator/
    * \brief Sobrecarga el operador / para dividir el vector
    * por otro vector, componente a componente.
    * \param Vector v : vector por el cual voy a dividir
    * \return Vector
    */
    Vector operator/(Vector v);


    /** \fn operator*
    * \brief Sobrecarga el operador * para realizar el
    * producto entre un vector y un real.
    * \param double d : Real por el cual voy a multiplicar.
    * \return Vector
    */
    Vector operator*(double d);

    /** \fn operator/
    * \brief Sobrecarga el operador / para dividir un
    * vector por un real.
    * \param double d : Real por el cual voy a dividir el vector.
    * \return Vector
    */
    Vector operator/(double d);

    /** \fn operator+
    * \brief Sobrecarga el operador + para sumarle un
    * real a cada componente del vector
    * \param double d : Real que voy a sumar.
    * \return Vector
    */
    Vector operator+(double d);

    /** \fn operator-
    * \brief Sobrecarga el operador - para restarle un
    * real a cada componente del vector
    * \param double d : Real que voy a restar.
    * \return Vector
    */
    Vector operator-(double d);

    /** \fn modulo
    * \brief Devuelve el módulo del vector
    * \return double
    */
    double modulo();

    /** \fn normaliza
    * \brief Devuelve el vector normalizado en otro vector.
    * No modifica el vector sobre el cual estamos trabajando.
    * \return Vector
    */
    Vector normaliza(Vector& v);

    /** \fn normalizame
    * \brief Normaliza el vector del argumento.
    */
    void normalizame(Vector* v);

    /** \fn operator<<
    * \brief Sobrecarga el operador inserción para que imprima un
    * vector.
    * \param std::ostream& out
    * \param const Vector& v : Vector que va a imprimir.
    * \return std::ostream&
    */
    friend std::ostream& operator<<(std::ostream& out, const Vector& v);

    /** \fn operator-
    * \brief Sobrecarga el operador - para restarle un
    * vector a un real.
    * \param double d : Real al que le voy a restar el vector.
    * \param Vector v : Vector que voy a restar.
    * \return Vector
    */
    friend Vector operator-(double d, Vector v); // Resta d - v

    // y = y + a*x
	friend void axpy( Vector* v, double& a , Vector& vv );

	//Vector operator=(Vector v);
};

void operator+=( std::vector<double>& v1, const std::vector<double>& v2 );
void operator-=( std::vector<double>& v1, const std::vector<double>& v2 );
void operator/=( std::vector<double>& v1, const std::vector<double>& v2 );
void operator*=( std::vector<double>& v1, const double& a );
void operator*=( std::vector<double>& v1, const std::vector<double>& v2 );
void operator/=( std::vector<double>& v1, const double& a );

// y = y + a*x
void axpy( std::vector<double>* y, double& a , std::vector<double>& x );
// y = y + a.*x
void axpy( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x );
// y = y - a*x
void naxpy( std::vector<double>* y, double& a , std::vector<double>& x );
// y = y - a.*x
void naxpy( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x );
double norm_squared( const std::vector<double>& v );
double norm_squared( const Vector& v );

double norma( const std::vector<double>& v );
double norma( const Vector& v );

#endif
