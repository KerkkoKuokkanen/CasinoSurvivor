
#include "screenTransition.h"

ScreenTransition::ScreenTransition()
{
	cover = new Image("everyColor", {0.0f, 0.0f, 25.0f, 15.0f}, 0.0f, 15);
	cover->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	cover->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	cover->drawActive = false;
}

void ScreenTransition::Init(void *data, size_t size)
{
	AddToSaveTracking(&transitionType, sizeof(int));
	CreateInputField("transition type", n_VarType::INTEGER, &transitionType);
	if (size == 0)
		return ;
	transitionType = *(int*)data;
	if (transitionType == 1)
		cover->position.x = 26.0f;
}

ScreenTransition::~ScreenTransition()
{
	delete cover;
}

void ScreenTransition::Update()
{
	cover->drawActive = true;
	if (time <= 0.0f && removeSelf)
		return (RemoveSelf());
	if (transitionType <= 1)
	{
		if (transitionType == 1 && cover->position.x <= 0.05f)
		{
			time -= DeltaTimeReal();
			return ;
		}
		cover->position = {cover->position.x - 44.0f * DeltaTimeReal(), cover->position.y};
	}
	time -= DeltaTimeReal();
}

void ScreenTransition::EngineUpdate()
{
	cover->drawActive = false;
}
