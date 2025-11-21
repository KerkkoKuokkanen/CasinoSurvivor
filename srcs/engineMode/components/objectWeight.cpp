
#include "objectWeight.h"

void ObjectWeight::Init(void *data, size_t size)
{
	CreateInputField("Weight", n_VarType::FLOAT, &weight);
	AddToSaveTracking(&weight, sizeof(float));
	if (size <= 1)
		return ;
	weight = *(float*)data;
}

void ObjectWeight::Start()
{
	if (self == NULL)
		return ;
	self->weight = weight;
}
