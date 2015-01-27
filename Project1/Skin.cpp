////////////////////////////////////////
// Skin.cpp
////////////////////////////////////////

#include "Header.h"

////////////////////////////////////////////////////////////////////////////////
Skin::Skin()
{

}

////////////////////////////////////////////////////////////////////////////////

Skin::~Skin()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Skin::load(const char *file) {
	/*Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");

	// Parse tree
	root = new Joint();
	root->load(token);

	//root->print();
	// Finish
	token.Close();*/
	return true;
}

void Skin::update()
{
	// Compute skinning matrix for each joint: M = W*B^-1
	// Loop thru vertices and compute blended position and normal
}

void Skin::draw()
{
	// Set GL matrix state to identity (world)
	// Loop thu triangles and draw using world space positions
	// and normals
}
