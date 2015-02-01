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
		o_positions.push_back(Vector3(x, y, z)); // Original positions must be stored for transformations
		n_positions.push_back(Vector3(x, y, z)); // This will be modified, for update
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
		o_normals.push_back(Vector3(x, y, z));
		n_normals.push_back(Vector3(x, y, z));
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
		temp_mat.Inverse();
		bindings.push_back(temp_mat);
	}
	token.FindToken("}");

	// Finish
	token.Close();
	return true;
}

void Skin::update(Skeleton skellington)
{
	Vector3 vertex, normal, new_v, acc_p, acc_n;
	int j_id = 0;
	float j_weight = 0.0;
	Matrix34 temp;
	std::vector<Matrix34> M;  // W (joint world) * B^-1

	// Compute skinning matrix for each joint: M = W*(B^-1)
	for (int j = 0; j < skellington.all_joints.size(); j++)
	{
		temp.Dot((skellington.all_joints[j])->WorldMtx, bindings[j]); // M = W * B^-1
		M.push_back(temp);
	}

	// Loop thru vertices and compute blended position and normal
	for (int i = 0; i < o_positions.size(); i++)
	{
		vertex = o_positions[i];
		normal = o_normals[i];
		acc_n.Set(0, 0, 0);  // Reset accumulator variables
		acc_p.Set(0, 0, 0);
		for (int j = 0; j < skin_weights[i].weights.size(); j+=2)
		{
			j_id = skin_weights[i].weights[j]; // get joint id
			j_weight = skin_weights[i].weights[j + 1]; // get weight for joint

			M[j_id].Transform(vertex, new_v);				// new_v = M * v
			new_v *= j_weight;               //  w (weight) * new_v
			acc_p += new_v;

			M[j_id].Transform3x3(normal, new_v);
			new_v *= j_weight;               //  w (weight) * W * B^-1 * v
			acc_n += new_v;
		}
		n_positions[i].Set(acc_p.x, acc_p.y, acc_p.z);
		acc_n = acc_n.Normalize();
		n_normals[i].Set(acc_n.x, acc_n.y, acc_n.z);
	}

}

void Skin::draw()
{
	// Set GL matrix state to identity (world)
	glLoadIdentity();
	// Loop thu triangles and draw using world space positions and normals
	Vector3 tri_vec;
	Vector3 pos_vec;
	Vector3 nor_vec;
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < triangles.size(); i++)
	{
		tri_vec = triangles[i];
		nor_vec = n_normals[tri_vec.x];
		pos_vec = n_positions[tri_vec.x];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = n_normals[tri_vec.y];
		pos_vec = n_positions[tri_vec.y];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);

		nor_vec = n_normals[tri_vec.z];
		pos_vec = n_positions[tri_vec.z];
		glNormal3f(nor_vec.x, nor_vec.y, nor_vec.z);
		glVertex3f(pos_vec.x, pos_vec.y, pos_vec.z);
	}
	glEnd();
}

void Skin::reset()
{
	o_positions.clear();
	n_positions.clear();
	o_normals.clear();
	n_normals.clear();
	triangles.clear();
	skin_weights.clear();
	bindings.clear();
}