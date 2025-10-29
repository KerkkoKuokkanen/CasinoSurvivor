
#ifndef IMAGE_H
# define IMAGE_H

# include "renderSystem.h"

struct textData {
	float x;
	float y;
	float w;
	float h;
	float a;
};

class Image : public RenderObj
{
	private:
		uint64_t textIndex = 0;
		GLuint texture = 0;
		t_Box color = {1.0f, 1.0f, 1.0f, 1.0f};
		textData textureData = {0.0f, 0.0f, 1.0f, 1.0f, 0.0f};
		int transformType = n_TransformTypes::TRANSFORM_CAMERA;
		t_BoundingB bBox = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};
		bool bBoxSet = false;
	public:
		bool active = true;
		t_Point position = {0.0f, 0.0f};
		t_Point dimentions = {0.0f, 0.0f};
		float angle = 0.0f;
		Image(uint64_t texture, t_Box rect, float angle, int layer);
		Image(std::string texture, t_Box rect, float angle, int layer);
		size_t GetSaveDataSize();
		void *CollectSaveData(void *buffer, size_t buffSize, size_t &size);
		void SetTransformType(int tType) {transformType = tType;};
		void SetLayer(int layer) {ChangeLayer(layer);};
		void SetTexture(std::string name);
		void SetTexture(uint64_t hahs);
		void SetColor(float r, float g, float b, float a);
		t_BoundingB getBoundingBox();
		void SetBoundingBox(t_BoundingB bbox);
		t_Box GetColor() {return (color);};
		uint64_t GetTexure() {return (textIndex);};
		GLuint GetGLTexture() {return (texture);};
		int GetLayer() {return (layer);};
		int GetTransformType() {return (transformType);};

		void SetTextureData(float x, float y, float w, float h, float a);
		textData GetTextureData();

		virtual void *GetImageComponent() override;
};

#endif
