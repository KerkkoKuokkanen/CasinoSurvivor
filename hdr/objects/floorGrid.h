
#ifndef FLOOR_GRID_H
# define FLOOR_GRID_H

# include "customComponent.h"
# include "componentRegistry.h"
# include "image.h"

# define GRID_WIDTH 58
# define GRID_HEIGHT 28
# define MAX_FREQUENCY 80000

struct GridPoint {
	t_Point home;
	t_Point current;
	bool atHome;
	t_Point force;
	float mag;
};

struct AppliedForce {
	t_Point position;
	float radius;
	float strength;
};

class FloorGrid : public CustomComponent
{
	private:
		Image *grid[GRID_HEIGHT][GRID_WIDTH] = {0};
		GridPoint points[GRID_HEIGHT + 1][GRID_WIDTH + 1] = {0};
		std::vector<AppliedForce> forces;
		std::vector<float> frequencies;
		void AssignBoundingBox();
		void AddForceToPoints();
		void CalculateForces();
		t_Point TransformPoint(t_Point point);
		void SetColorForGrid(int i, int j, int bar);
	public:
		FloorGrid();
		~FloorGrid();
		t_Box GetColorMatchingPoint(float x);
		void GiveFrequencies(std::vector<float> &freq);
		void ApplyForce(t_Point position, float strength, float radius) {forces.push_back({TransformPoint(position), radius, strength});};
		void Start() override;
		void Update() override;
};

REGISTER_COMPONENT(FloorGrid);

#endif
