
#include "shopBackGround.h"
#include "keyboard.h"

#define IMAGE_AMOUNT 5000
#define X_MAX 20.0f

struct BGSC {
	float r;
	float g;
	float b;
};

struct colSet {
	float r1;
	float g1;
	float b1;
	float r2;
	float g2;
	float b2;
};

static std::vector<colSet> finalColors5 = {
	{0.472854, 0.257378, 0.755575, 0.947831, 0.192873, 0.615185},
	{0.411584, 0.494046, 0.432164, 0.379637, 0.562297, 0.522304},
	{0.360340, 0.237476, 0.266273, 0.252525, 0.184067, 0.620240}
};

static std::vector<colSet> finalColors6 = {
	{0.512236, 0.155160, 0.779280, 0.361845, 0.521055, 0.377750},
	{0.849011, 0.330204, 0.745036, 0.827358, 0.414185, 0.200890},
	{0.365392, 0.148965, 0.660331, 0.191201, 0.511859, 0.813492}
};

static std::vector<colSet> finalColors7 = {
	{0.549322, 0.455637, 0.893200, 0.008389, 0.993631, 0.955570},
	{0.267224, 0.232578, 0.933729, 0.190415, 0.311161, 0.683920},
	{0.648317, 0.263032, 0.776902, 0.384328, 0.396180, 0.589291}
};

static int usedTextureSet = 0;

static BGSC GetImageColor()
{
	usedTextureSet = rand() % 3;
	float divider = 1.25f;
	colSet cols;
	switch (usedTextureSet)
	{
		case 0:
			cols = finalColors5[rand() % 3];
			break ;
		case 1:
			cols = finalColors6[rand() % 3];
			divider = 2.0f;
			break ;
		case 2:
			cols = finalColors7[rand() % 3];
			divider = 2.5f;
			break ;
	}
	float rScale = cols.r1 - cols.r2;
	float gScale = cols.g1 - cols.g2;
	float bScale = cols.b1 - cols.b2;
	float r = cols.r1 + rScale * float_rand();
	float g = cols.g1 + gScale * float_rand();
	float b = cols.b2 + bScale * float_rand();
	return ((BGSC){r / divider, g / divider, b / divider});
}

ShopBackGround::ShopBackGround()
{
	images.reserve(IMAGE_AMOUNT);
	for (int i = 0; i < IMAGE_AMOUNT; i++)
	{
		float xPos = -20.0f + float_rand() * 40.0f;
		float yPos = -16.0f + float_rand() * 32.0f;
		float xSize = 2.0f + float_rand() * 2.0f;
		float ySize = 2.0f + float_rand() * 2.0f;
		float cycle = float_rand() * PI * 2.0f;
		float angle = sin(cycle) * 0.2f;
		Image *img = new Image("shopBackGround", {xPos, yPos, xSize, ySize}, angle, 2);
		img->drawDepth = float_rand();
		BGSC col = GetImageColor();
		img->SetColor(col.r, col.g, col.b, 1.0f);
		images.push_back({img, 0.6f + float_rand() * 0.6f, cycle});
	}
}

ShopBackGround::~ShopBackGround()
{
	for (int i = 0; i < images.size(); i++)
		delete images[i].img;
}

void ShopBackGround::Update()
{
	for (int i = 0; i < images.size(); i++)
	{
		float angle = sin(images[i].cycle) * 0.33f;
		images[i].img->angle = angle;
		images[i].img->position.x += images[i].speed * DeltaTime() * 0.15f;
		images[i].img->dimentions.x += angle * 0.01f;
		images[i].img->dimentions.y += angle * 0.01f;
		images[i].cycle += DeltaTime() * 0.4f;
		if (images[i].cycle >= PI * 2.0f)
			images[i].cycle = 0.0f;
		if (images[i].img->position.x >= X_MAX)
			images[i].img->position.x = -X_MAX;
	}
}
