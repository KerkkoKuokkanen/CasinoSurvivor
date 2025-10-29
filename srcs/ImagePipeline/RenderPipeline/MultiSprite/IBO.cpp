
#include "IBO.h"
#include "ShaderClass.h"

Shader *IBOShader = NULL;
static int scaleLocationWH = 0;

void IBO::CreateEBO()
{
	glGenBuffers(1, &instanceEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instanceEBO);

	static const GLuint index[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

	glBindVertexArray(instanceVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instanceEBO);
}

IBO::~IBO()
{
	if (instanceVAO)
	{
		glDeleteVertexArrays(1, &instanceVAO);
		instanceVAO = 0;
	}
	if (instanceVBO)
	{
		glDeleteBuffers(1, &instanceVBO);
		instanceVBO = 0;
	}
	if (instanceEBO)
	{
		glDeleteBuffers(1, &instanceEBO);
		instanceEBO = 0;
	}
}

IBO::IBO(uint32_t maxSize, GLuint texture)
{
	IBO::maxSize = maxSize;
	IBO::texture = texture;

	glGenVertexArrays(1, &instanceVAO);
	glBindVertexArray(instanceVAO);

	glGenBuffers(1, &instanceVBO);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

	CreateEBO();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
	glVertexAttribDivisor(0, 1);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, texUV));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, scale));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, angle));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)offsetof(InstanceData, color));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
}

void IBO::UpdateInstances(std::vector<InstanceData> &updatedInstances)
{
	size = (uint32_t)updatedInstances.size();
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

	glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, updatedInstances.size() * sizeof(InstanceData), updatedInstances.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IBO::Draw(float w, float h)
{
	glUseProgram(IBOShader->ID);
	glBindVertexArray(instanceVAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform2f(scaleLocationWH, w, h);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, size);

	glBindVertexArray(0);
	glUseProgram(0);
}

void InitIBO()
{
	IBOShader = new Shader("shaders/multi_sprite_vert.glsl", "shaders/multi_sprite_frag.glsl");
	scaleLocationWH = glGetUniformLocation(IBOShader->ID, "uUniformScale");
}
