////////////////////////////////////////
// Dof.h
////////////////////////////////////////

#ifndef DOF_H
#define DOF_H

#include "Header.h"

////////////////////////////////////////////////////////////////////////////////

class Dof {
public:
	Dof();
	~Dof();
	void setPose(float new_pose);
	void setMin(float new_min);
	void setMax(float new_max);
	float getMin();
	float getMax();
	float getPose();
	void update();
	void draw();

private:

	// Constants
	float pose = 0;
	float min = -100000;
	float max = 100000;
	Matrix34 WorldMtx;
	Matrix34 LocalMtx;
};

////////////////////////////////////////////////////////////////////////////////

#endif
