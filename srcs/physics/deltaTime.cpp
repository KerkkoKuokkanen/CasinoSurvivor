
float deltaTime = 0.0f;
float deltaMulti = 1.0f;

void SetDeltaTime(float time)
{
	if (time > 0.1f)
		deltaTime = 0.1f;
	else
		deltaTime = time;
}

float DeltaTimeReal()
{
	return (deltaTime);
}

float DeltaTime()
{
	return (deltaTime * deltaMulti);
}

void SetDeltaMulti(float mult)
{
	deltaMulti = mult;
}

void InitDeltaTime(unsigned int frameRate)
{
	deltaTime = 1.0f / (float)frameRate;
}
