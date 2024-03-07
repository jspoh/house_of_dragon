#ifndef PARTICLE_MANAGER
#define PARTICLE_MANAGER
#include "SingletonTemplate.h"
#include "Particle.h"
#include "utils.h"

class ParticleManager : public Singleton<ParticleManager>
{
	friend Singleton<ParticleManager>;
public:
	virtual ~ParticleManager() {}

	void init_particle( const char* fileName);
	void emit_enemy_death_particle( Point deadEnemyPos);
	void update_particles( float dt);

	//void AddParticle(...);
private:
	//Make a pooling system
	ParticleManager() {}
	const int particlesNum = 200;
	Particle* particleArr;


	enum PARTICLE_TYPE {
		Enemy_death
	};



};


#endif