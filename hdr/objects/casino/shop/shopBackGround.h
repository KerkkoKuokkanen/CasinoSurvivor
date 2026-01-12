
#ifndef SHOP_BACKGROUND_H
# define SHOP_BACKGROUND_H

# include "component.h"

struct BackGroundImageData {
	Image *img;
	float speed;
	float cycle;
};

class ShopBackGround : public CustomComponent
{
	private:
		std::vector<BackGroundImageData> images;
	public:
		ShopBackGround();
		~ShopBackGround();
		void Update() override;
};

REGISTER_COMPONENT(ShopBackGround);

#endif
