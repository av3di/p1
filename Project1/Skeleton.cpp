////////////////////////////////////////
// Skeleton.cpp
////////////////////////////////////////

#include "Skeleton.h"

////////////////////////////////////////////////////////////////////////////////
Skeleton::Skeleton()
{
	LocalMtx.Identity();
}

////////////////////////////////////////////////////////////////////////////////

Skeleton::~Skeleton()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Skeleton::load(const char *file) {
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	
	// Parse tree
	root = new Joint();
	root->load(token);
	
	//root->print();
	// Finish
	token.Close();
	return true;
}

void Skeleton::update()
{
	Matrix34 Wp;
	Wp.Identity();
	root->update(Wp);
}

void Skeleton::draw()
{
	Matrix34 Wp;
	Wp.Identity();
	root->draw();
}
