#pragma once
//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>
#include <iostream>

class Shader
{
public:
	Shader(const GLchar* , const GLchar* );
	~Shader();
	void USE();
	GLuint Program;
};

