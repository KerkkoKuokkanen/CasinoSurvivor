
float deltaTime = 0.0f;

void SetDeltaTime(float time)
{
	if (time > 0.1f)
		deltaTime = 0.1f;
	else
		deltaTime = time;
}

float DeltaTime()
{
	return (deltaTime);
}

void InitDeltaTime(unsigned int frameRate)
{
	deltaTime = 1.0f / (float)frameRate;
}
