////////////////////////////////////////
// Skeleton.h
////////////////////////////////////////

#ifndef SKELETON_H
#define SKELETON_H


#include "Header.h"

////////////////////////////////////////////////////////////////////////////////

class Skeleton {
public:
	Skeleton();
	~Skeleton();
	bool load(const char *file);
	void update();
	void draw();

	void init_all_joints(Joint* current);
	std::vector<Joint*> all_joints;
	Joint *root;
private:
	// Constants

	Matrix34 LocalMtx;
};

////////////////////////////////////////////////////////////////////////////////



#endif

