
float deltaTime = 0.0f;
float timeSpeed = 1.0f;

void SetSpeed(float speed)
{
	timeSpeed = speed;
}

float DeltaTime()
{
	return (deltaTime * timeSpeed);
}

void InitDeltaTime(unsigned int frameRate)
{
	deltaTime = 1.0f / (float)frameRate;
}
