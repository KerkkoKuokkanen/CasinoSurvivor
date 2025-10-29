
#include "ShaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw "error";
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];

	if (std::strcmp(type, "VERTEX") == 0 || std::strcmp(type, "FRAGMENT") == 0) {
		// Check shader compilation status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (!hasCompiled) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else if (std::strcmp(type, "PROGRAM") == 0) {
		// Check program linking status
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (!hasCompiled) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// in these next six lines of code we create the shaders to be used in the shader program
	// this for the positions. Vertex part translates 3d pos to 2d
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// fragment part is for the colors
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	//this is the program
	ID = glCreateProgram();

	//here we attatch the previously created shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// here we link the previously compiled shaders to the program to be used
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID); //tells opengl that shader program should be used for the coming rendering commands
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}
