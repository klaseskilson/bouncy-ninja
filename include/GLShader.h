#ifndef GLSHADER_H_
#define GLSHADER_H_

/*
 * Shader loading from nexcius.net:
 * http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
 */

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class GLShader{
	
public:
	GLuint programID;

	GLShader();

	//Constructor - create and compile both a vertex and fragment shader at once
	GLShader(const char* vertexshaderfile, const char* fragmentshaderfile);

	// Destructor
	~GLShader();


private:
	std::string GLShader::readFile(const char *filePath);

	GLuint LoadShader(const char *vertex_path, const char *fragment_path);


};
#endif // GLSHADER_H_
