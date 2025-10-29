
#ifndef SHADER_CLASS_H
# define SHADER_CLASS_H

# include <OpenGL/gl3.h>
# include <string>
# include <fstream>
# include <sstream>
# include <iostream>
# include <cerrno>

# define PI 3.1415926

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	void compileErrors(unsigned int shader, const char* type);

	void Activate();
	void Delete();
};

#endif
