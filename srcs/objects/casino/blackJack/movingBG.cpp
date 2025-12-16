
#include "movingBG.h"
#include "deltaTime.h"

void MovingBG::Start()
{
	img = (Image*)self->GetComponent("image");
}

void MovingBG::Update()
{
	if (img == NULL)
		return ;
	textData dat = img->GetTextureData();
	if (dat.x <= -1.0f)
	{
		dat.x = 0.0f;
		dat.y = 0.0f;
	}
	img->SetTextureData(dat.x - 0.025f * DeltaTime(), dat.y - 0.025f * DeltaTime(), dat.w, dat.h, dat.a);
}
