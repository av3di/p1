////////////////////////////////////////
// Shader.h
////////////////////////////////////////

#ifndef SHADER_H
#define SHADER_H


#include "Header.h"

////////////////////////////////////////////////////////////////////////////////

class Shader {
public:
	Shader();
	~Shader();
	char* readFile(const char* filename);
	void makeVertexShader(const char* shadersource);
	void makeFragmentShader(const char* shadersource);
	void makeShaderProgram();

	GLuint vs_id = 0;  // vertex shader id
	GLuint fs_id = 0;  // fragment shader id
	GLuint ps_id = 0; // program shader id
};

////////////////////////////////////////////////////////////////////////////////



#endif

