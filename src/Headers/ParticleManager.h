#pragma once

#include "SingletonTemplate.h"
#include "Particle.h"
#include "utils.h"




class ParticleManager : public Singleton<ParticleManager>
{
	friend Singleton<ParticleManager>;
public:
	ParticleManager();
	
	//void init_particle(const char* texturePath);
	void init_particle();
	void emit_Particle(Point cursorPos);
	void update_particles(float dt);
	void death_particle(Point deadEnemyPos);
	void render_particles();
	void create_particles();
private:
	
	~ParticleManager();


	std::string circle1;

	struct Particle_Prop
	{
		Point position;
		Point velocity;
		Point size;
		f32 lifetime;
		f32 alpha;
		bool active = false;
		s32 type;

	};


	static const int particlesNum = 100;
	Particle_Prop particleArr[particlesNum]{ 0 };

	enum PARTICLE_TYPE
	{
		enemyDeath,
		tracingCursor
	};
	s32 numParticles = 200;

};

