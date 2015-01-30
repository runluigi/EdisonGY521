/*
 * Vector.cpp
 *
 *  Created on: Jan 17, 2015
 *      Author: luis
 */

#include "Vector.h"

Vector::Vector() {
	// TODO Auto-generated constructor stub
	x=0;
	y=0;
	z=0;
}

Vector::~Vector() {
	// TODO Auto-generated destructor stub
}

int Vector::getX(void){
	return x;
}

int Vector::getY(void){
	return y;
}
int Vector::getZ(void){
	return z;
}
void Vector::setXYZ(int _x,int _y, int _z){
	x=_x;
	y=_y;
	z=_z;
return;
}
