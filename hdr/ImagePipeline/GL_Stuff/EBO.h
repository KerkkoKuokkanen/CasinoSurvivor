
#ifndef EBO_CLASS_H
# define EBO_CLASS_H

# include <OpenGL/gl3.h>
# include <vector>

class EBO
{
	public:
		GLuint ID;
		void Create(std::vector<GLuint>& indecies);
		void Bind();
		void Unbind();
		void Delete();
};

#endif
