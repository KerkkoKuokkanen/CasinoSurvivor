
#include "screenTransition.h"

ScreenTransition::ScreenTransition()
{
	cover = new Image("everyColor", {0.0f, 0.0f, 25.0f, 15.0f}, 0.0f, 15);
	cover->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	cover->SetTransformType(n_TransformTypes::TRANSFORM_STATIC);
	cover->drawActive = false;
}

ScreenTransition::~ScreenTransition()
{
	delete cover;
}

void ScreenTransition::Update()
{
	cover->drawActive = true;
	if (time <= 0.0f)
		return (RemoveSelf());
	cover->position = {cover->position.x - 38.0f * DeltaTimeReal(), cover->position.y};
	time -= DeltaTimeReal();
}

void ScreenTransition::EngineUpdate()
{
	cover->drawActive = false;
}
