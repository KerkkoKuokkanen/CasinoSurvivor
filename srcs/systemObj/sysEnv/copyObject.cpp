
#include "copyObject.h"
#include "componentRegistry.h"

void *CreateImageComponent(void *data, size_t size)
{
	size_t offset = 0;
	uint8_t *castData = (uint8_t*)data;
	float posX = *(float*)(castData + offset); offset += sizeof(float);
	float posY = *(float*)(castData + offset); offset += sizeof(float);
	float dimX = *(float*)(castData + offset); offset += sizeof(float);
	float dimY = *(float*)(castData + offset); offset += sizeof(float);
	float angle = *(float*)(castData + offset); offset += sizeof(float);
	float r = *(float*)(castData + offset); offset += sizeof(float);
	float g = *(float*)(castData + offset); offset += sizeof(float);
	float b = *(float*)(castData + offset); offset += sizeof(float);
	float a = *(float*)(castData + offset); offset += sizeof(float);
	uint64_t text = *(uint64_t*)(castData + offset); offset += sizeof(uint64_t);
	int tType = *(int*)(castData + offset); offset += sizeof(int);
	int layer = *(int*)(castData + offset); offset += sizeof(int);
	float tpx = *(float*)(castData + offset); offset += sizeof(float);
	float tpy = *(float*)(castData + offset); offset += sizeof(float);
	float tdx = *(float*)(castData + offset); offset += sizeof(float);
	float tdy = *(float*)(castData + offset); offset += sizeof(float);
	float ta = *(float*)(castData + offset); offset += sizeof(float);
	Image *img = new Image(text, {posX, posY, dimX, dimY}, angle, layer);
	img->SetColor(r, g, b, a);
	img->SetTransformType(tType);
	img->SetTextureData(tpx, tpy, tdx, tdy, ta);
	return ((void*)img);
}

SystemObj *CopyObject(SystemObj *source)
{
	if (source == NULL)
		return (NULL);
	SystemObj *copy = new SystemObj();
	source->ResetComponentSaveFetching();
	copy->saveable = source->saveable;
	while (source->ComponentFetchingAtEnd() == false)
	{
		uint32_t componentType = source->FetchComponentClassType();
		size_t componentSize = source->FetchComponentDataSize();
		void *componentData = malloc(componentSize);
		void *ret = source->FetchComponentSaveData(componentData, componentSize, componentSize);
		if (componentType >= 999) //The component type starting number for custom components
			memcpy(componentData, ret, componentSize);
		else
			componentData = ret;
		if (componentType == n_ComponentTypes::IMAGE_CLASS)
		{
			void *img = CreateImageComponent(componentData, componentSize);
			copy->AddComponent(img, componentType);
		}
		else
		{
			std::string used = GetComponentNameWithKey(componentType);
			copy->AddComponent(used, componentData, componentSize);
		}
		free(componentData);
		source->IncrimentComponentFetching();
	}
	return (copy);
}
