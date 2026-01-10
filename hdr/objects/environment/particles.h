
#ifndef PARTICLES_H
# define PARTICLES_H

# include "component.h"

# define MAX_PARTICLES 50000

struct Particle {
	Image *image;
	t_Point dir;
	float speed;
	float life;
	float fade;
	float slowMult;
};

class Particles : public CustomComponent
{
	private:
		std::vector<Particle> particles;
	public:
		Particles();
		~Particles();
		void AddParticles(t_Point pos, float area, t_Point dir, float spread,
							std::vector<t_Box> &colors, float speedS, float speedB,
							int amount, std::string texture, float sizeS, float sizeB,
							float lifeS, float lifeB, float slowMulti);
		void Update() override;
};

REGISTER_COMPONENT(Particles);

#endif
