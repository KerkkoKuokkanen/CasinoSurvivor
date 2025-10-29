
#include "shapeEX.h"
#include "commonTools.h"

Vertex *GLShapeEX::BindOwn()
{
	mesh.VAO.Bind();
	mesh.VBO.Bind();
	Vertex *vertData = (Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	return (vertData);
}

void GLShapeEX::UnBindOwn()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void GLShapeEX::SetTextRotOwn(Vertex *vertData, float angle)
{
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	for (int i = 0; i < vertexAmount; i++)
	{
		float uvX = vertData[i].texUV.x;
		float uvY = vertData[i].texUV.y;
		float rotatedX = uvX * cosAngle - uvY * sinAngle;
		float rotatedY = uvX * sinAngle + uvY * cosAngle;
		vertData[i].texUV.x = rotatedX;
		vertData[i].texUV.y = rotatedY;
	}
}

void GLShapeEX::SetTextureDistance(Vertex *vertData, float width, float height)
{
	sDistance = {width, height};
	for (int i = 0; i < vertexAmount; i++)
	{
		float uvX = (ogVertexData)[i].texUV.x;
		float uvY = (ogVertexData)[i].texUV.y;
		float distVecX = (uvX - center.x) * width;
		float distVecY = (uvY - center.y) * height;
		vertData[i].texUV.x = distVecX;
		vertData[i].texUV.y = distVecY;
	}
}

void GLShapeEX::SetTexturePosition(Vertex *vertData, float x, float y)
{
	textPosition = {x, y};
	float currentX = 0.0f;
	float currentY = 0.0f;
	for (int i = 0; i < vertexAmount; i++)
	{
		currentX += vertData[i].texUV.x;
		currentY += vertData[i].texUV.y;
	}
	currentX /= vertexAmount;
	currentY /= vertexAmount;
	float xAdd = -x - currentX;
	float yAdd = y - currentY;
	float cosAngle = cos(textAngle);
	float sinAngle = sin(textAngle);
	float rotatedX = xAdd * cosAngle - yAdd * sinAngle;
	float rotatedY = xAdd * sinAngle + yAdd * cosAngle;
	for (int i = 0; i < vertexAmount; i++)
	{
		vertData[i].texUV.x += rotatedX;
		vertData[i].texUV.y += rotatedY;
	}
}

void GLShapeEX::SetTextureRotation(Vertex *vertData, float angle)
{
	SetTextRotOwn(vertData, angle);
	GLShapeEX::textAngle = angle;
}

void GLShapeEX::SetAll(float x, float y, float width, float height, float angle)
{
	Vertex *vertData = BindOwn();
	SetTextureDistance(vertData, width, height);
	SetTextureRotation(vertData, angle);
	SetTexturePosition(vertData, x, y);
	UnBindOwn();
}

GLShapeEX::GLShapeEX(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType)
						: GLShape(verts, inds, texture, shader, boundingBox, useType)
{
	center = GetPivotPoint();
	vertexAmount = verts.size();
	defaultEdges = boundingBox;
	ogVertexData = verts;
	ogIndexData = inds;
}

GLShapeEX::~GLShapeEX()
{

}

t_DataForShape GLShapeEX::GetCurrentShapeDataSet()
{
	t_DataForShape ret;
	ret.bBox = defaultEdges;
	ret.indexData = ogIndexData;
	Vertex *vertData = BindOwn();
	std::vector<Vertex> vertsForUse;
	for (int i = 0; i < vertexAmount; i++)
		vertsForUse.push_back(vertData[i]);
	UnBindOwn();
	ret.vertexData = vertsForUse;
	return (ret);
}
