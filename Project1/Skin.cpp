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
	Tokenizer token;
	int num_of_vec = 0; // Number of vectors to be stored
	float x = 0;
	float y = 0;
	float z = 0;

	token.Open(file);

	// Get the positions
	token.FindToken("positions");
	num_of_vec = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < num_of_vec; i++)
	{
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		positions.push_back(Vector3(x, y, z));
	}
	token.FindToken("}");

	// Get the normals
	token.FindToken("normals");
	num_of_vec = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < num_of_vec; i++)
	{
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		normals.push_back(Vector3(x, y, z));
	}
	token.FindToken("}");

	// Get the triangles
	token.FindToken("triangles");
	num_of_vec = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < num_of_vec; i++)
	{
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		triangles.push_back(Vector3(x, y, z));
	}
	token.FindToken("}");

	// Finish
	token.Close();
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
	glLoadIdentity();
	// Loop thu triangles and draw using world space positions and normals
	Vector3 tri_vec;
	Vector3 pos_vec;
	Vector3 nor_vec;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++)
	{
		tri_vec = triangles[i];
		nor_vec = normals[tri_vec.x];
		pos_vec = positions[tri_vec.x];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = normals[tri_vec.y];
		pos_vec = positions[tri_vec.y];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = normals[tri_vec.z];
		pos_vec = positions[tri_vec.z];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);
	}
	glEnd();
}
