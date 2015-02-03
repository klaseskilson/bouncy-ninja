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

GLuint LoadShader(const char *vertex_path, const char *fragment_path);

#endif // GLSHADER_H_
