////////////////////////////////////////
// Joint.cpp
////////////////////////////////////////

#include "Joint.h"

////////////////////////////////////////////////////////////////////////////////
Joint::Joint()
{
	LocalMtx.Identity();
	WorldMtx.Identity();
}

////////////////////////////////////////////////////////////////////////////////
Joint::~Joint()
{
}

bool Joint::load(Tokenizer &token)
{
	token.FindToken("{");
	float x = 0;
	float y = 0;
	float z = 0;
	float rot_min = 0;
	float rot_max = 0;
	Vector3 tmp_v;
	while (1) {
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			tmp_v.Set(x, y, z);
			setOffset(tmp_v);
		}
		else if (strncmp(temp, "boxmin", 256) == 0)
		{
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			tmp_v.Set(x, y, z);
			setBoxMin(tmp_v);
		}
		else if (strncmp(temp, "boxmax", 256) == 0)
		{
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			tmp_v.Set(x, y, z);
			setBoxMax(tmp_v);
		}
		else if (strncmp(temp, "pose", 256) == 0)
		{
			x = token.GetFloat();
			y = token.GetFloat();
			z = token.GetFloat();
			// If pose out of limits, then it clamps
			getDofX().setPose(x);
			getDofY().setPose(y);
			getDofZ().setPose(z);
		}
		else if (strncmp(temp, "rotxlimit", 256) == 0)
		{
			rot_min = token.GetFloat();
			rot_max = token.GetFloat();
			getDofX().setMin(rot_min);
			getDofX().setMax(rot_max);
		}
		else if (strncmp(temp, "rotylimit", 256) == 0)
		{
			rot_min = token.GetFloat();
			rot_max = token.GetFloat();
			getDofY().setMin(rot_min);
			getDofY().setMax(rot_max);
		}
		else if (strncmp(temp, "rotzlimit", 256) == 0)
		{
			rot_min = token.GetFloat();
			rot_max = token.GetFloat();
			getDofZ().setMin(rot_min);
			getDofZ().setMax(rot_max);
		}
		else if (strcmp(temp, "balljoint") == 0) {
				Joint *jnt = new Joint();
				jnt->load(token);
				setChild(*jnt);
		}
		else if (strcmp(temp, "}") == 0) return true;
		else token.SkipLine();      // Unrecognized token
	}
}

////////////////////////////////////////////////////////////////////////////////

void Joint::setName(char* new_name)
{
	strcpy(name, new_name);
}

void Joint::setChild(Joint child)
{
	children.push_back(child);
}

void Joint::setOffset(Vector3 off)
{
	offset = off;
}

void Joint::setBoxMin(Vector3 min)
{
	boxmin = min;
}

void Joint::setBoxMax(Vector3 max)
{
	boxmax = max;
}

void Joint::setDofX(Dof x)
{
	dofx = x;
}

////////////////////////////////////////////////////////////////////////////////

void Joint::setDofY(Dof y)
{
	dofy = y;
}

////////////////////////////////////////////////////////////////////////////////

void Joint::setDofZ(Dof z)
{
	dofz = z;
}

////////////////////////////////////////////////////////////////////////////////

Vector3 Joint::getDof()
{
	return Vector3(dofx.getPose(), dofy.getPose(), dofz.getPose());
}

Dof& Joint::getDofX()
{
	return dofx;
}

Dof& Joint::getDofY()
{
	return dofy;
}
Dof& Joint::getDofZ()
{
	return dofz;
}


////////////////////////////////////////////////////////////////////////////////

void Joint::update(Matrix34 &Wp)
{
	// Compute LocalMatrix
	Matrix34 rotatex, rotatey, rotatez;
	rotatex.MakeRotateX(dofx.getPose());
	rotatey.MakeRotateY(dofy.getPose());
	rotatez.MakeRotateZ(dofz.getPose());
	LocalMtx.Dot(rotatex, LocalMtx);
	LocalMtx.Dot(rotatey, LocalMtx);
	LocalMtx.Dot(rotatez, LocalMtx);

	Matrix34 translate;
	translate.MakeTranslate(offset);
	LocalMtx.Dot(translate, LocalMtx);

	// Compute WorldMatrix
	WorldMtx.Dot(Wp, LocalMtx);

	// Recursively call update() on children
	for (int i = 0; i < children.size(); i++)
		children[i].update(WorldMtx);
}

////////////////////////////////////////////////////////////////////////////////

void Joint::draw()
{
	// Do some OpenGL
	glLoadMatrixf(WorldMtx);
	drawBox(boxmin, boxmax);

	// Recursively call Draw() on children
	for (int i = 0; i < children.size(); i++)
		children[i].draw();
}

void Joint::drawBox(Vector3 boxMin, Vector3 boxMax)
{
	// Draw wire cube
	drawWireBox(boxmin.x, boxmin.y, boxmin.z, boxmax.x, boxmax.y, boxmax.z);
}

void Joint::print()
{
	//std::cout << "Joint: " << name << std::endl;
	offset.Print("offset is ");
	boxmin.Print("boxmin is ");
	boxmax.Print("boxmax is ");
	std::cout << "dofx pose is " << dofx.getPose() << std::endl;
	std::cout << "dofy pose is " << dofy.getPose() << std::endl;
	std::cout << "dofz pose is " << dofz.getPose() << std::endl;
	for (int i = 0; i < children.size(); i++)
		children[i].print();
}