
#include "camera.h"
#include "mouse.h"
#include "envHandler.h"

void Camera::Init(void *data, size_t size)
{
	CreateInputField("x", n_VarType::FLOAT, &x);
	CreateInputField("y", n_VarType::FLOAT, &y);
	CreateInputField("zoom", n_VarType::FLOAT, &zoom);
	CreateInputField("Apply", n_VarType::BOOL, &apply);
	if (size <= 1)
		return ;
	char *cast = (char*)data;
	x = READ_AND_ADVANCE(cast, float);
	y = READ_AND_ADVANCE(cast, float);
	zoom = READ_AND_ADVANCE(cast, float);
	SetCameraCoordinates(x, y);
	SetScreenSpaceDimentions(zoom, zoom);
	SaveCameraPosition();
}

void Camera::SetCameraPosition(float x, float y)
{
	Camera::x = x;
	Camera::y = y;
	SetCameraCoordinates(x, y);
}

void Camera::SetCameraZoom(float zoom)
{
	Camera::zoom = zoom;
	SetScreenSpaceDimentions(zoom, zoom);
}

void Camera::SaveCameraPosition()
{
	ClearSaveData();
	AddToSave(&x, sizeof(float));
	AddToSave(&y, sizeof(float));
	AddToSave(&zoom, sizeof(float));
}

void Camera::EngineUpdate()
{
	if (apply)
	{
		apply = false;
		ClearSaveData();
		AddToSave(&x, sizeof(float));
		AddToSave(&y, sizeof(float));
		AddToSave(&zoom, sizeof(float));
	}

	SetCameraCoordinates(x, y);
	SetScreenSpaceDimentions(zoom, zoom);

	if (!OverImgui())
	{
		if (WheelIn())
			zoom *= 0.98f;
		else if (WheelOut())
			zoom *= 1.02f;
	}

	if (MouseKeyHeld(n_MouseKeys::MOUSE_LEFT) && moving)
	{
		t_Point place = GetMouseXYZoom();
		x = offSet.x - (place.x - pressPlace.x);
		y = offSet.y - (place.y - pressPlace.y);
	}
	else
		moving = false;
	if (MouseKeyPressed(n_MouseKeys::MOUSE_LEFT) && !OverImgui())
	{
		pressPlace = GetMouseXYZoom();
		offSet = {x, y};
		moving = true;
	}
}
