
#include "particles.h"
#include "vectorTools.h"

Particles::Particles()
{
	particles.reserve(MAX_PARTICLES);
}

Particles::~Particles()
{
	for (int i = 0; i < particles.size(); i++)
		delete particles[i].image;
}

void Particles::AddParticles(t_Point pos, float area, t_Point dir, float spread,
							std::vector<t_Box> &colors, float speedS, float speedB,
							int amount, std::string texture, float sizeS, float sizeB,
							float lifeS, float lifeB, float slowMulti)
{
	float diff = sizeB - sizeS;
	float diffS = speedB - speedS;
	float diffL = lifeB - lifeS;
	t_Point vec = VectorNormalize(dir);
	if (particles.size() + amount > MAX_PARTICLES)
		amount = MAX_PARTICLES - particles.size();
	for (int i = 0; i < amount; i++)
	{
		float size = float_rand() * diff + sizeB;
		float speed = float_rand() * diffS + speedS;
		float life = float_rand() * diffL + lifeS;
		if (life < 0.0001f)
			continue ;
		float angle = float_rand() * spread;
		angle = (rand() % 2 == 0) ? angle : -angle;
		t_Point d = VectorRotate(vec, angle);
		t_Point sc = RandSinAndCos();
		float r = float_rand() * area;
		float pX = pos.x + r * sc.x;
		float pY = pos.y + r * sc.y;
		Image *img = new Image(texture, {pX, pY, size, size}, 0.0f, 12);
		t_Box color = colors[rand() % colors.size()];
		img->SetColor(color.x, color.y, color.w, color.h);
		particles.push_back({img, d, speed, life, life * 0.5f, slowMulti});
	}
}

void Particles::Update()
{
	for (Particle &p : particles)
	{
		t_Point pos = p.image->position;
		pos.x += p.dir.x * p.speed * DeltaTime();
		pos.y += p.dir.y * p.speed * DeltaTime();
		p.image->position = pos;
		p.speed = p.speed * (1.0f - DeltaTime() * p.slowMult);
		if (p.life < p.fade)
		{
			float scale = 1.0f / p.fade;
			float a = scale * p.life;
			t_Box col = p.image->GetColor();
			p.image->SetColor(col.x, col.y, col.w, a);
		}
		p.life -= DeltaTime();
	}
}
