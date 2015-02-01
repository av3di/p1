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
	Skinweight sw;
	Matrix34 temp_mat;

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

	// Get the skin weights
	token.FindToken("skinweights");
	num_of_vec = token.GetInt();
	int attachments = 0;
	token.FindToken("{");
	for (int i = 0; i < num_of_vec; i++) // Get every row of skinweights
	{
		attachments = token.GetInt();
		for (int j = 0; j < (attachments * 2); j++)  // Get all floats in that row
			sw.weights.push_back(token.GetFloat());
		skin_weights.push_back(sw); // Add the skinweight to the Skin's list of skinweights
		sw.weights.clear(); // Clear it for resuse in next iteration
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

	// Get the binding matrices
	token.FindToken("bindings");
	num_of_vec = token.GetInt(); // Get the number of matrices
	token.FindToken("{");
	for (int i = 0; i < num_of_vec; i++)
	{
		token.FindToken("matrix");
		token.FindToken("{");

		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		temp_mat.a.Set(x, y, z); // first row

		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		temp_mat.b.Set(x, y, z); // second row

		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		temp_mat.c.Set(x, y, z); // thrid row

		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		temp_mat.d.Set(x, y, z); // fourth row

		token.FindToken("}");
		bindings.push_back(temp_mat);
	}
	token.FindToken("}");
	for (int i = 0; i < bindings.size(); i++)
		bindings[i].Print("bind matrix: ");

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
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++)
	{
		tri_vec = triangles[i];
		nor_vec = normals[tri_vec.x];
		pos_vec = positions[tri_vec.x];
		nor_vec = nor_vec.Normalize();
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = normals[tri_vec.y];
		pos_vec = positions[tri_vec.y];
		nor_vec = nor_vec.Normalize();
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = normals[tri_vec.z];
		pos_vec = positions[tri_vec.z];
		nor_vec = nor_vec.Normalize();
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);
	}
	glEnd();
}
