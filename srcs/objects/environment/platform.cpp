
#include "platform.h"

void Platform::Start()
{
	std::vector<void*> images = self->GetComponents("image");
	for (int i = 0; i < images.size(); i++)
	{
		Image *imag = (Image*)images[i];
		uint64_t sign = imag->GetTexure();
		if (sign == GetTextureGLData("platform3").hash)
		{
			img = imag;
			break ;
		}
	}
}

void Platform::Update()
{
	if (img == NULL)
		return ;
	textData dat = img->GetTextureData();
	if (dat.x > 10000.0f)
		dat.x = 0.0f;
	img->SetTextureData(dat.x - 0.08f * DeltaTime(), dat.y, dat.w, dat.h, dat.a);
}
