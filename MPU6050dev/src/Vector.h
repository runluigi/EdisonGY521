/*
 * Vector.h
 *
 *  Created on: Jan 17, 2015
 *      Author: luis
 */

#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
private:
	int x,y,z;
public:
	Vector();
	Vector(int x,int y, int z);
	virtual ~Vector();
	int getX();
	int getY();
	int getZ();
	void setXYZ(int x,int y,int z);
};

#endif /* VECTOR_H_ */
