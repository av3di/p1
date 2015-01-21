////////////////////////////////////////
// Joint.h
////////////////////////////////////////

#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include "vector3.h"
#include "matrix34.h"
#include "Dof.h"
#include "token.h"
#include "core.h"

////////////////////////////////////////////////////////////////////////////////

class Joint {
public:
	Joint();
	~Joint();
	bool load(Tokenizer &t);
	void setName(char* new_name);
	void setChild(Joint child);
	void setOffset(Vector3 off);
	void setBoxMin(Vector3 min);
	void setBoxMax(Vector3 max);
	void setDofX(Dof x);
	void setDofY(Dof y);
	void setDofZ(Dof z);
	Vector3 getDof();
	Dof& getDofX();
	Dof& getDofY();
	Dof& getDofZ();
	void update(Matrix34 &Wp);
	void draw();
	void drawBox(Vector3 boxmax, Vector3 boxmin);
	void print();

private:
	// Constants
	char name[100];
	std::vector<Joint> children;
	Vector3 offset;
	Vector3 boxmin, boxmax;
	Dof dofx;
	Dof dofy;
	Dof dofz;

	Matrix34 WorldMtx;
	Matrix34 LocalMtx;
};

////////////////////////////////////////////////////////////////////////////////

#endif
