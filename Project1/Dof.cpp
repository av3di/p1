////////////////////////////////////////
// Dof.cpp
////////////////////////////////////////

#include "Header.h"

////////////////////////////////////////////////////////////////////////////////
Dof::Dof()
{
}

////////////////////////////////////////////////////////////////////////////////
Dof::~Dof()
{
}

////////////////////////////////////////////////////////////////////////////////
void Dof::setMin(float new_min)
{
	min = new_min;
}
void Dof::setMax(float new_max)
{
	max = new_max;
}

void Dof::setPose(float new_pose)
{
	if (new_pose < min)
		pose = min;
	else if (new_pose > max)
		pose = max;
	else
		pose = new_pose;
}
float Dof::getMin()
{
	return min;
}
float Dof::getMax()
{
	return max;
}
float Dof::getPose()
{
	return pose;
}

////////////////////////////////////////////////////////////////////////////////
void Dof::update()
{

}

void Dof::draw()
{

}