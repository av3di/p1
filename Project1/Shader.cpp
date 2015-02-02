////////////////////////////////////////
// Shader.cpp
////////////////////////////////////////

#include "Header.h"

////////////////////////////////////////////////////////////////////////////////
Shader::Shader()
{

}

////////////////////////////////////////////////////////////////////////////////

Shader::~Shader()
{
}

////////////////////////////////////////////////////////////////////////////////
char* Shader::readFile(const char* filename)
{
	// Open the file
	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror("Error opening file!");
		exit(0);
	}
	
	// Move the file pointer to the end of the file and determining the length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[file_length + 1];
	// zero out memory
	for (int i = 0; i < file_length + 1; i++)
	{
		contents[i] = 0;
	}
	// Here's the actual read
	fread(contents, 1, file_length, fp);
	// This is how you denote the end of a string in C
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}
void Shader::makeVertexShader(const char* shadersource)
{
	vs_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_id, 1, (const GLchar **)&shadersource, NULL);
	glCompileShader(vs_id);
}

void Shader::makeFragmentShader(const char* shadersource)
{
	fs_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_id, 1, (const GLchar **)&shadersource, NULL);
	glCompileShader(fs_id);
}

void Shader::makeShaderProgram()
{
	ps_id = glCreateProgram();
	glAttachShader(ps_id, vs_id);
	glAttachShader(ps_id, fs_id);
	glLinkProgram(ps_id);
}