#pragma once

// Library Includes - NOTE: Load order is important, GLEW must be included before GLFW
#include <glew.h> 
#include <glfw3.h>
#include <iostream>



class ShaderLoader
{
	
public:	
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char *filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};