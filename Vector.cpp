/*! \file Vector.cpp
 * \brief Archivo de implementación de la clase Vector.
 * \author Luciana Luque <lu.m.luque@gmail.com>
 * \version 1.0
 * \date 2020.06.12
 */
#include "Vector.h"
using namespace std;

Vector::Vector(): x(0.0), y(0.0), z(0.0) {}

Vector::Vector(double x1, double y1, double z1) : x(x1), y(y1), z(z1) {}

Vector Vector::operator+(Vector v)
{
    double x1, y1, z1;
    x1 = x + v.x;
    y1 = y + v.y;
    z1 = z + v.z;
    return Vector(x1, y1, z1);
}

Vector Vector::operator-(Vector v)
{
    double x1, y1, z1;
    x1 = x - v.x;
    y1 = y - v.y;
    z1 = z - v.z;
    return Vector(x1, y1, z1);
}

double Vector::operator*(Vector v)
{
    double x1, y1, z1;
    x1 = x * v.x;
    y1 = y * v.y;
    z1 = z * v.z;
    return (x1 + y1 + z1);
}

Vector Vector::operator/(Vector v)
{
    double x1, y1, z1;
    x1 = x / v.x;
    y1 = y / v.y;
    z1 = z / v.z;
    return Vector(x1, y1, z1);
}

Vector Vector::operator*(double d)
{
    double x1, y1, z1;
    x1 = x * d;
    y1 = y * d;
    z1 = z * d;
    return Vector(x1, y1, z1);
}

Vector Vector::operator/(double d)
{
    double x1, y1, z1;
    x1 = x / d;
    y1 = y / d;
    z1 = z / d;
    return Vector(x1, y1, z1);
}

double Vector::modulo(){
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector Vector::operator+(double d)
{
    double x1, y1, z1;
    x1 = x + d;
    y1 = y + d;
    z1 = z + d;
    return Vector(x1, y1, z1);
}

Vector Vector::operator-(double d)
{
    double x1, y1, z1;
    x1 = x - d;
    y1 = y - d;
    z1 = z - d;
    return Vector(x1, y1, z1);
}

Vector Vector::normaliza(Vector& v)
{
	Vector output = v;
    double norm = 0.0;

    norm += v.x * v.x;
    norm += v.y * v.y;
    norm += v.z * v.z;
    norm = sqrt( norm );

    output.x = v.x / norm;
    output.y = v.y / norm;
    output.z = v.z / norm;

    static bool te_lo_adverti = false;
    if( norm <= 1e-16 ){

        if( te_lo_adverti == false ){

            std::cout << "Advertencia: El vector es muy chiquito por lo que se \
            lo normalizó a 0" << std::endl << std::endl;

            te_lo_adverti = true;
        }

        output.x = 0.0;
        output.y = 0.0;
        output.z = 0.0;
    }

    return output;
}

void Vector::normalizame(Vector* v)
{
    double norm = 1e-32;

    norm += v->x * v->x;
    norm += v->y * v->y;
    norm += v->z * v->z;
    norm = sqrt( norm );

    v->x = v->x / norm;
    v->y = v->y / norm;
    v->z = v->z / norm;

    
    static bool te_lo_adverti = false;
    if( norm <= 1e-16 ){

        if( te_lo_adverti == false ){

            std::cout << "Advertencia: El vector es muy chiquito por lo que se \
            lo normalizó a 0" << std::endl << std::endl;

            te_lo_adverti = true;
        }

        v->x = 0.0;
        v->y = 0.0;
        v->z = 0.0;
    };

}

std::ostream& operator<<(std::ostream& out, const Vector& v)
{
    out << v.x << ", ";
    out << v.y << ", ";
    out << v.z << " ";
    out << endl;
    return out;
}

Vector operator-(double d, Vector v)
{
    double x1, y1, z1;
    x1 = d - v.x;
    y1 = d - v.y;
    z1 = d - v.z;
    return Vector(x1, y1, z1);
}

void axpy( Vector* v, double& a , Vector& vv )
{
	v->x += a * vv.x;
	v->y += a * vv.y;
	v->z += a * vv.z;

 return ;
}



void operator+=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] += v2[i]; }
 return;
}

void operator-=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] -= v2[i]; }
 return;
}

void operator/=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] /= v2[i]; }
 return;
}

void operator*=( std::vector<double>& v1, const double& a )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] *= a; }
 return;
}

void operator*=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] *= v2[i]; }
 return;
}

void operator/=( std::vector<double>& v1, const double& a )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] /= a; }
 return;
}

void axpy( std::vector<double>* y, double& a , std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] += a * x[i] ;
 }
 return ;
}

void axpy( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] += a[i] * x[i] ;
 }
 return;
}


void naxpy( std::vector<double>* y, double& a , std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] -= a * x[i] ;
 }
 return ;
}

void naxpy( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] -= a[i] * x[i] ;
 }
 return;
}

double norm_squared( const std::vector<double>& v )
{
 double out = 0.0;
 for( unsigned int i=0 ; i < v.size() ; i++ )
 { out += ( v[i] * v[i] ); }
 return out;
}

double norm_squared( const Vector& v )
{
 double out = 0.0;

 out += v.x * v.x;
 out += v.y * v.y;
 out += v.z * v.z;

 return out;
}

double norma(const Vector& v){

    return sqrt( norm_squared( v ) );

}

double norma( const std::vector<double>& v ){

    return sqrt( norm_squared( v ) );

}
