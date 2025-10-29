
#ifndef VBO_CLASS_H
# define VBO_CLASS_H

# include <OpenGL/gl3.h>
# include <glm/glm.hpp>
# include <vector>

typedef struct Vertex
{
	glm::vec3 position;
	glm::vec2 texUV;
}				Vertex;

class VBO
{
public:
	GLuint ID;
	void Create(std::vector<Vertex>& vertecies, GLenum usage);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
