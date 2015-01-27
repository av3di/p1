////////////////////////////////////////
// Skeleton.cpp
////////////////////////////////////////

#include "Header.h"

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
	char the_name[256];
	token.GetToken(the_name);
	root->setName(the_name);
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

void Skeleton::init_all_joints(Joint* current)
{
	all_joints.push_back(current);
	for (int i = 0; i < current->children.size(); i++)
	{
		init_all_joints(&(current->children[i]));
	}
}
