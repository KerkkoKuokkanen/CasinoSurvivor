
#ifndef CROSS_HAIR_H
# define CROSS_HAIR_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

class CrossHair : public CustomComponent
{
	private:
		Image *img[5] = {NULL, NULL, NULL, NULL, NULL};
		float recoil = 0.0f;
	public:
		~CrossHair();
		void SetRecoil(float gunRecoil) {recoil = gunRecoil;};
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(CrossHair);

#endif
