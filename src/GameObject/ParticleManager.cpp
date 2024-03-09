#include "ParticleManager.h"
#include <fstream>

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void  ParticleManager::init_particle(const char* fileName)
{
	
	RenderHelper::getInstance()->registerTexture( circle1 , ".Assets/Particle_Effects/Single Particles/PNG (Black background) delete/circle_01.png");
	std::ifstream ifs(fileName); // Open an input file stream with the provided fileName

	if (ifs.is_open())
	{
		for (int i = 0; i < particlesNum; ++i)
		{
			// Read particle properties from the file
			ifs >> particleArr[i].position.x >> particleArr[i].position.y
				>> particleArr[i].velocity.x >> particleArr[i].velocity.y
				>> particleArr[i].size.x
				>> particleArr[i].size.y
				>> particleArr[i].lifetime
				>> particleArr[i].alpha
				>> particleArr[i].active;
		}

		ifs.close(); // Close the file stream
	}
	else
	{
		// Error opening the file
		// Handle the error appropriately
	}
}

void ParticleManager::render_particles()
{
	for (int i = 0; i < particlesNum; ++i)
	{

		RenderHelper::getInstance()->texture(circle1, particleArr[i].position.x, particleArr[i].position.y, particleArr[i].size.x, particleArr[i].size.x, particleArr[i].alpha, (Color{0,1,0,0}), 0);

	}



}

void ParticleManager::create_particles()
{
	// Set default values for particle properties
	Point emitterPos = { 0.0f, 0.0f }; // Default emitter position
	int particleCount = 10; // Default particle count
	float speed = 100.0f; // Default particle speed
	float sizeMin = 5.0f; // Default minimum particle size
	float sizeMax = 10.0f; // Default maximum particle size
	float lifetime = 1.0f; // Default particle lifetime
	PARTICLE_TYPE type = tracingCursor; // Default particle type

	int particlesEmitted = 0;

	for (int i = 0; i < particlesNum && particlesEmitted < particleCount; ++i)
	{
		if (!particleArr[i].active)
		{
			particleArr[i].position = emitterPos;
			particleArr[i].size.x = sizeMin + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (sizeMax - sizeMin)));
			particleArr[i].size.y = particleArr[i].size.x;
			particleArr[i].alpha = 1.0f;
			particleArr[i].lifetime = lifetime;
			particleArr[i].active = true;
			particleArr[i].type = type;

			float angle = static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX) * 2.0f * PI;
			particleArr[i].velocity.x = speed * cos(angle);
			particleArr[i].velocity.y = speed * sin(angle);

			++particlesEmitted;
		}
	}
}




void ParticleManager::death_particle(Point deadEnemyPos)
{

	const int MAX_PARTICLES_PER_BURST = 100;
	const float PARTICLE_SPEED_MIN = 100.0f;
	const float PARTICLE_SPEED_MAX = 250.0f;
	const float PARTICLE_SIZE_MIN = 5.0f;
	const float PARTICLE_SIZE_MAX = 15.0f;
	const float PARTICLE_LIFETIME = 1.2f;

	int particlesEmittedThisBurst = 0;
	float angleIncrement = (2.0f * PI) / static_cast<f32>(MAX_PARTICLES_PER_BURST);
	float currentAngle = 0.0f;

	for (int i = 0; i < particlesNum; ++i) {
		if (!particleArr[i].active) {
			if (particlesEmittedThisBurst < MAX_PARTICLES_PER_BURST) {
				float speed = PARTICLE_SPEED_MIN + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (PARTICLE_SPEED_MAX - PARTICLE_SPEED_MIN)));
				//particles[i].position = enemyDeathPosition;
				particleArr[i].size.x = PARTICLE_SIZE_MIN + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (PARTICLE_SIZE_MAX - PARTICLE_SIZE_MIN)));
				particleArr[i].size.y = PARTICLE_SIZE_MIN + static_cast<f32>(rand()) / (static_cast<f32>(RAND_MAX / (PARTICLE_SIZE_MAX - PARTICLE_SIZE_MIN)));
				particleArr[i].alpha = 1.0f;
				particleArr[i].lifetime = PARTICLE_LIFETIME;
				particleArr[i].active = true;
				particleArr[i].type = enemyDeath;

				particleArr[i].velocity.x = speed * cosf(currentAngle);
				particleArr[i].velocity.y = speed * sinf(currentAngle);

				currentAngle += angleIncrement;
				++particlesEmittedThisBurst;
			}
			else {
				break;
			}
		}
	}
}

// update each particle's position, size etc using their values
void ParticleManager::update_particles(float deltaTime)
{
	for (int i{}; i < particlesNum; i++) {
		if (particleArr[i].active) { // for each active particle
			particleArr[i].position.x += particleArr[i].velocity.x * deltaTime; // calculate new position using velocity
			particleArr[i].position.y += particleArr[i].velocity.y * deltaTime; // calculate new position using velocity
			particleArr[i].size.x -= deltaTime * 20.f; // smaller over time
			particleArr[i].size.y -= deltaTime * 20.f; // smaller over time
			particleArr[i].alpha -= deltaTime; // more transparent over time
			particleArr[i].lifetime -= deltaTime;
			if (particleArr[i].lifetime <= 0.0f) { // when lifetime reaches 0
				particleArr[i].active = false; // particle is no longer active
			}
		}
	}
	render_particles();
}



void ParticleManager::emit_Particle(Point cursorPos) {

	for (int i = 0; i < numParticles; i++)
	{
		if (!particleArr[i].active)
		{
			particleArr[i].position.x = cursorPos.x + (AERandFloat() * 20.f - 10.f); // -10 to 10
			particleArr[i].position.y = cursorPos.y - (30.f + AERandFloat() * 20.f); // 30 to 50
			particleArr[i].size.x = 10.f + AERandFloat() * 10.f; // different sizes
			particleArr[i].size.y = 10.f + AERandFloat() * 10.f; // different sizes
			particleArr[i].alpha = 1.f;
			particleArr[i].lifetime = 0.2f;
			particleArr[i].active = true;
			particleArr[i].type = tracingCursor;

			float particleSpeed = 100.f;
			particleArr[i].velocity.x = particleSpeed * ((float)(rand() % 101 - 50) / 100.0f); // random velocity between -0.5 to 0.5
			particleArr[i].velocity.y = particleSpeed * ((float)(rand() % 101 - 50) / 100.0f); // random velocity between -0.5 to 0.5

			break; // Exit the loop after emitting a particle
		}

	}


}