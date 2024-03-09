#pragma once

#include "SingletonTemplate.h"
#include "Particle.h"
#include "utils.h"




class ParticleManager : public Singleton<ParticleManager>
{
	friend Singleton<ParticleManager>;
public:
	ParticleManager();
	

	void init_particle(const char* fileName);
	void emit_Particle(Point cursorPos);
	void update_particles(float dt);
	void death_particle(Point deadEnemyPos);
	void render_particles();
	void ParticleManager::create_particles();
private:
	//Make a pooling system
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
	Particle_Prop particleArr[particlesNum];

	enum PARTICLE_TYPE
	{
		enemyDeath,
		tracingCursor
	};
	s32 numParticles = 200;

};

