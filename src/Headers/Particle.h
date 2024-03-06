//#pragma once
//#ifndef PARTICLE
//#define PARTICLE
//
//
//class ParticleManager;
//class Particle : public GameObject
//{
//public:
//	Particle();
//	~Particle();
//private:
//	Particle(ParticleManager* _ParticleMgr); // This is used to register to ParticleManager
//	static Particle* sInstance; // The pointer to the object that gets registered
//
//	const int maxParticles = 200;
//
//	// enum for different types of particle
//	enum PARTICLE_TYPE {
//		PLAYER_WALKING,
//		ENEMY_DEATH
//	};
//	struct Point
//	{
//		float x, y;
//	};
//	// particle texture
//
//
//	// particle object
//	typedef struct {
//		Point position;
//		Point velocity;
//		float size;
//		float lifetime;
//		float alpha;
//		bool active;
//		int type;
//	}Particle;
//	
//	//Add your variables here
//};
//
//void init_particle(Particle* particleArr, const char* fileName);
//void emit_enemy_death_particle(Particle* particleArr, Point deadEnemyPos);
//void update_particles(Particle* particleArr, float dt);
//
//#endif