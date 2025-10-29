
#ifndef INCTANCE_BUFFER_OBJECT_H
# define INCTANCE_BUFFER_OBJECT_H

# include "VAO.h"
# include "box.h"

struct InstanceData {
	t_Point position;
	t_Box texUV;
	t_Point scale;
	float angle;
	t_Box color;
	uint32_t key;
	t_Point useScale;
};

class IBO
{
	private:
		uint32_t maxSize;
		glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
		GLuint texture;
		GLuint instanceEBO;
		GLuint instanceVBO;
		GLuint instanceVAO;
		uint32_t size = 0;
		void CreateEBO();
	public:
		IBO(uint32_t maxSize, GLuint texture);
		~IBO();
		void UpdateInstances(std::vector<InstanceData> &updatedInstances);
		void Draw(float w, float h);
};

void InitIBO();

#endif
