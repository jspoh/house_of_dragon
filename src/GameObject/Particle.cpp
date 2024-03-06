//#include "GameObject/Particle.h"
//
//Particle::Particle()
//{
//}
//
//Particle::Particle(ParticleManager* _ParticleMgr)
//{
//	//_ParticleMgr->AddParticle("SceneBase", this);
//}
//
//Particle::~Particle()
//{
//}
//
//
//void init_particle(Particle* particleArr, const char* fileName)
//{
//	std::ifstream ifs{ fileName }; // open an input file stream with provided FileName
//
//	std::string temp;
//
//	float floatZero; // file I/O 0.f
//	bool boolFalse; // false
//
//	ifs >> temp >> floatZero >> temp >> boolFalse;
//
//	for (int i{}; i < maxParticles; ++i) { // for each particle, initialize all values to 0
//		particleArr[i].position = { floatZero, floatZero };
//		particleArr[i].velocity = { floatZero, floatZero };
//		particleArr[i].size = floatZero;
//		particleArr[i].lifetime = floatZero;
//		particleArr[i].alpha = floatZero;
//		particleArr[i].active = boolFalse;
//	}
//
//	ifs.close();
//}
//
//
//
//// function to emit "explosion" particles at enemy's position when slain
//void emit_enemy_death_particle(Particle* particleArr, Point deadEnemyPos)
//{
//	int numOfParticles = 30; // particls per explosion
//	int particlesGenerated = 0;
//
//	float angleIncrememt = (2 * PI) / numOfParticles; // to generate particles in a circle
//	float angle = 0.f; // start from angle 0
//
//	for (int i{}; i < maxParticles; i++) {
//		if (!particleArr[i].active) { // for each non-active particle
//			if (particlesGenerated < numOfParticles) {
//				float particleSpeed = ((float)(rand() % 151 + 150) / 1.0f); // 150.f to 300.f different speeds for vfx
//
//				particleArr[i].position = deadEnemyPos;
//				particleArr[i].size = 10.f + AERandFloat() * 10.f; // 10.f to 20.f
//				particleArr[i].alpha = 1.f;
//				particleArr[i].lifetime = 0.4f;
//				particleArr[i].active = true;
//				particleArr[i].type = ENEMY_DEATH;
//				particleArr[i].velocity = { particleSpeed * cosf(angle), particleSpeed * sinf(angle) };
//
//				angle += angleIncrememt; // next particle's direction will be slightly incremented for circular explosion
//				++particlesGenerated; // particle counter
//			}
//			else {
//				break;
//			}
//		}
//	}
//}
//
//// update each particle's position, size etc using their values
//void update_particles(Particle* particleArr, float deltaTime) 
//{
//	for (int i{}; i < maxParticles; i++) {
//		if (particleArr[i].active) { // for each active particle
//			particleArr[i].position.x += particleArr[i].velocity.x * deltaTime; // calculate new position using velocity
//			particleArr[i].position.y += particleArr[i].velocity.y * deltaTime; // calculate new position using velocity
//			particleArr[i].size -= deltaTime * 20.f; // smaller over time
//			particleArr[i].alpha -= deltaTime; // more transparent over time
//			particleArr[i].lifetime -= deltaTime;
//			if (particleArr[i].lifetime <= 0.0f) { // when lifetime reaches 0
//				particleArr[i].active = false; // particle is no longer active
//			}
//		}
//	}
//}
