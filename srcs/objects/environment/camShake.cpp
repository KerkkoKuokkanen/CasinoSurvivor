
#include "camShake.h"
#include "envHandler.h"

void CameraShake::Start()
{
	SystemObj *obj = FindSystemObject(4874219225160765108LU);
	cam = (Camera*)obj->GetComponent("Camera");
}

void CameraShake::CreateCameraShake(float shakeTime)
{
	time = shakeTime;
	if (time < 0.04f)
		time = 0.04f;
	cam->SetCameraZoom(10.0f);
	iter = 1.0f / time;
}

void CameraShake::CreateCameraShakeDefault()
{
	time = 0.05f;
	cam->SetCameraZoom(10.0f);
	iter = 1.0f / time;
}

void CameraShake::Update()
{
	if (time <= 0.0f)
	{
		cam->SetCameraZoom(10.0f);
		return ;
	}
	float y = iter * -time + 0.5f;
	cam->SetCameraZoom(10.0f + y);
	time -= DeltaTimeReal();
}
