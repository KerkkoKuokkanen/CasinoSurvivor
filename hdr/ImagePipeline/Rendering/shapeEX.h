
#ifndef SHAPE_EX_H
# define SHAPE_EX_H

# include "shape.h"

class GLShapeEX : public GLShape
{
	private:
		Vertex *BindOwn();
		void UnBindOwn();
		t_Point center;
		int vertexAmount;
		t_BoundingB defaultEdges;
		std::vector<Vertex> ogVertexData;
		std::vector<GLuint> ogIndexData;
		void SetTextRotOwn(Vertex *vertData, float angle);
		void SetTextureDistance(Vertex *vertData, float width, float height);
		void SetTexturePosition(Vertex *vertData, float x, float y);
		void SetTextureRotation(Vertex *vertData, float angle);
		float textAngle = 0.0f;
		t_Point textPosition = {0.5f, 0.5f};
		t_Point sDistance = {1.0f, 1.0f};
	public:
		GLShapeEX(std::vector<Vertex> &verts, std::vector<GLuint> &inds, GLuint texture, Shader *shader, t_BoundingB boundingBox, int useType);
		void SetAll(float x, float y, float width, float height, float angle);
		t_DataForShape GetCurrentShapeDataSet();
		float GetTextureAngle() {return(textAngle);};
		t_Point GetTextureDistance() {return(sDistance);};
		t_Point GetTexturePosition() {return(textPosition);};
		~GLShapeEX();
};

#endif
