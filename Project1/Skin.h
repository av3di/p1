////////////////////////////////////////
// Skin.h
////////////////////////////////////////

#ifndef SKIN_H
#define SKIN_H


#include "Header.h"

////////////////////////////////////////////////////////////////////////////////

class Skin {
public:
	Skin();
	~Skin();
	bool load(const char *file);
	void update(Skeleton skellington);
	void draw();
	void reset(); // clear everything to parse a new skin file

private:
	std::vector<Vector3> o_positions, n_positions; // original vertices, new vertices
	std::vector<Vector3> o_normals, n_normals;
	std::vector<Vector3> triangles;
	std::vector<Skinweight> skin_weights;
	std::vector<Matrix34> bindings;   // Inverses of binding matrices
};

////////////////////////////////////////////////////////////////////////////////



#endif

