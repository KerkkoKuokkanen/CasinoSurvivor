
#ifndef CAM_SHAKE_H
# define CAM_SHAKE_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "camera.h"

class CameraShake : public CustomComponent
{
	private:
		Camera *cam = NULL;
		float time = 0.0f;
	public:
		void CreateCameraShake();
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(CameraShake);

#endif
