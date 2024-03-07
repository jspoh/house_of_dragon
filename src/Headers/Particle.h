#pragma once
#ifndef PARTICLE
#define PARTICLE

#include "ParticleManager.h"



class Particle 
{
public:
	~Particle();

	Particle(ParticleManager* _ParticleMgr);
	static Particle* sInstance;


	Point position;
	Point velocity;
	float size;
	float lifetime;
	float alpha;
	bool active;
	int type;




private:
	Particle();

};

#endif