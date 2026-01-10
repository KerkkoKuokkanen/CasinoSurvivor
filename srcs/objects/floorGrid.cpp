
#include "floorGrid.h"
#include "deltaTime.h"
#include "mouse.h"

#define MIN_VELOCITY 0.0005f
#define SPEED 3.0f

FloorGrid::FloorGrid()
{
	frequencies = std::vector<float>(29, 0.0f);
	float ys1 = -0.6f;
	float ys2 = -0.5f;
	float ww = 1.2f / 24;
	for (int i = 0; i < GRID_HEIGHT + 1; i++)
	{
		float ydiff = ww * i;
		for (int j = 0; j < GRID_WIDTH + 1; j++)
		{
			float xdiff = ww * j;
			points[i][j].home = {-1.45f + xdiff, ys1 + ydiff};
			points[i][j].current = points[i][j].home;
			points[i][j].atHome = true;
			points[i][j].force = {0.0f, 0.0f};
			points[i][j].mag = 0.0f;
		}
	}
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			Image *img = new Image("grid", {0.0f, 0.0f, 1.0f, 1.0f}, 0.0f, 1);
			img->position = {0.0f, 0.0f};
			img->SetColor(1.0f, 1.0f, 1.0f, 0.65f);
			grid[i][j] = img;
			grid[i][j]->SetBoundingBox({points[i][j].current, points[i][j + 1].current,
										points[i + 1][j + 1].current, points[i + 1][j].current});
		}
	}
}

void FloorGrid::Start()
{
	self->weight = 100.0f;
}

void FloorGrid::AssignBoundingBox()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			grid[i][j]->SetBoundingBox({points[i][j].current, points[i][j + 1].current,
										points[i + 1][j + 1].current, points[i + 1][j].current});
		}
	}
}

void FloorGrid::AddForceToPoints()
{
	for (AppliedForce f : forces)
	{
		float scale = 1.0f / f.radius;
		for (int i = 1; i < GRID_HEIGHT + 1; i++)
		{
			for (int j = 0; j < GRID_WIDTH + 1; j++)
			{
				float x = points[i][j].current.x - f.position.x;
				float y = points[i][j].current.y - f.position.y;
				float distSQ = x * x + y * y;
				float rSQ = f.radius * f.radius;
				if (distSQ > rSQ)
					continue ;
				if (fabs(x + y) < 0.00001f)
					continue ;
				float dist = sqrt(distSQ);
				float xn = x / dist;
				float yn = y / dist;
				float str = fmin(1.1f - scale * dist, 0.65f);
				points[i][j].force.x += xn * str * f.strength;
				points[i][j].force.y += yn * str * f.strength;
				points[i][j].atHome = false;
			}
		}
	}
	forces.clear();
}

void FloorGrid::CalculateForces()
{
	for (int i = 0; i < GRID_HEIGHT + 1; i++)
	{
		for (int j = 0; j < GRID_WIDTH + 1; j++)
		{
			if (points[i][j].atHome)
				continue ;
			float delta = DeltaTimeReal() * SPEED;
			points[i][j].current.x += points[i][j].force.x * delta;
			points[i][j].current.y += points[i][j].force.y * delta;
			float hx = points[i][j].home.x - points[i][j].current.x;
			float hy = points[i][j].home.y - points[i][j].current.y;
			points[i][j].force.x += hx * delta;
			points[i][j].force.y += hy * delta;
			if (fabs(points[i][j].force.x) + fabs(points[i][j].force.y))
			{
				points[i][j].force.x *= (1.0f / (1.0f + delta));
				points[i][j].force.y *= (1.0f / (1.0f + delta));
			}
			float dist = sqrt(hx * hx + hy * hy);
			points[i][j].mag = dist;
			if (dist < MIN_VELOCITY && (fabs(points[i][j].force.x) + fabs(points[i][j].force.y)) < MIN_VELOCITY)
			{
				points[i][j].current.x = points[i][j].home.x;
				points[i][j].current.y = points[i][j].home.y;
				points[i][j].atHome = true;
				points[i][j].mag = 0.0f;
			}
		}
	}
}

t_Point FloorGrid::TransformPoint(t_Point point)
{
	float x = point.x / 10.0f;
	float y = point.y / 10.0f;
	return ((t_Point){x, y});
}

void FloorGrid::Update()
{
	AddForceToPoints();
	CalculateForces();
	AssignBoundingBox();
}

t_Box FloorGrid::GetColorMatchingPoint(float x)
{
	float ux = TransformPoint({x, 0.0f}).x;
	float close = 999.9f;
	int closest = 0;
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		if (fabs(points[0][i].home.x - ux) < close)
		{
			closest = i;
			close = fabs(points[0][i].home.x - ux);
		}
	}
	return (grid[3][closest]->GetColor());
}

void FloorGrid::SetColorForGrid(int i, int j, int bar)
{
	int i2 = i * 2;
	static float x = 0.0f;
	float R = sin(x + ((float)i * 0.1f)) / 4.0f + 0.75f;
	float B = sin(x + (PI * 0.5f) + ((float)i * 0.1f)) / 4.0f + 0.75f;
	float G = sin(x + PI + ((float)i * 0.1f)) / 4.0f + 0.75f;
	float mag1 = points[j][i2].mag + points[j][i2 + 1].mag + points[j + 1][i2 + 1].mag + points[j + 1][i2].mag;
	float mag2 = points[j][i2 + 1].mag + points[j][i2 + 1 + 1].mag + points[j + 1][i2 + 1 + 1].mag + points[j + 1][i2 + 1].mag;
	float aAdd1 = 0.2f * fmin(1.0f, mag1 * 5.0f);
	float aAdd2 = 0.2f * fmin(1.0f, mag2 * 5.0f);
	if (j <= bar)
	{
		grid[j][i2]->SetColor(R, G, B, 0.75f + (aAdd1 / 2.5f));
		grid[j][i2 + 1]->SetColor(R, G, B, 0.75f + (aAdd2 / 2.5f));
	}
	else
	{
		grid[j][i2]->SetColor(R, G, B, 0.6f + aAdd1);
		grid[j][i2 + 1]->SetColor(R, G, B, 0.6f + aAdd2);
	}
	x += 0.00001f;
	if (x >= PI * 2)
		x = 0.0f;
}

void FloorGrid::GiveFrequencies(std::vector<float> &freq)
{
	std::vector<float> temp(freq.begin() + 10, freq.end() - 4);
	for (int i = 0; i < temp.size(); i++)
	{
		if (frequencies[i] < temp[i])
			frequencies[i] = temp[i];
		else
		{
			frequencies[i] -= (MAX_FREQUENCY / 35.0f);
			if (frequencies[i] < 0)
				frequencies[i] = 0.0f;
		}
	}
	for (int i = 0; i < frequencies.size(); i++)
	{
		int bar = (int)frequencies[i] / (MAX_FREQUENCY / 24);
		if (bar > 23)
			bar = 23;
		else if (bar < 0)
			bar = 0;
		for (int j = 0; j < GRID_HEIGHT; j++)
			SetColorForGrid(i, j, bar);
	}
}

FloorGrid::~FloorGrid()
{
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
			if (grid[i][j] != NULL)
				delete grid[i][j];
	}
}
