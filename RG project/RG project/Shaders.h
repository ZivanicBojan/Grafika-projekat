#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

GLuint compileShader(const char* shaderSource, GLenum shaderType);
GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif
