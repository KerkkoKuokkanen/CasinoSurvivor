
#include "camShake.h"
#include "envHandler.h"

void CameraShake::Start()
{
	SystemObj *obj = FindSystemObject(4874219225160765108LU);
	cam = (Camera*)obj->GetComponent("Camera");
}

void CameraShake::CreateCameraShake()
{
	time = 0.04f;
	cam->SetCameraZoom(10.0f);
}

void CameraShake::Update()
{
	if (time <= 0.0f)
	{
		cam->SetCameraZoom(10.0f);
		return ;
	}
	float y = 20.0f * time - 0.9f;
	cam->SetCameraZoom(10.0f + y);
	time -= DeltaTime();
}
