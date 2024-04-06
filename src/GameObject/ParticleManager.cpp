/* Start Header ************************************************************************/
/*!
\file ParticleManager.cpp
\author Yang yujie, yujie.yang, 2301383 (90%, particle logic implementation)
\par yujie.yang\@digipen.edu
\author Poh Jing Seng, jingseng.poh, 2301363 (10%, bugfixes, structure)
\par jingseng.poh\@digipen.edu
\date 01 Apr 2024
\brief particle manager implementation
 * This file contains the implementation of the particle manager which handles
 * creation, updating, and rendering of particles in the game. It supports various
 * types of particles including regular, explosion, and firework.
 *
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "pch.h"
#include "ParticleManager.h"
#include "MyMath.h"


/**
 * @class ParticleManager
 * @brief Manages particle effects within the game.
 *
 * Responsible for storing and managing the lifecycle and behavior of particles.
 * It includes functionality to create, update and render particles for visual effects.
 */
ParticleManager::ParticleManager() {
	m_particles.reserve(m_PROJECTED_MAX_PARTICLES);

	/*********************************************
	//PARTICLE TEXTURES REGISTRATION
	**********************************************/
	RenderHelper::getInstance()->registerTexture(REGULAR, "Assets/Particle_Effects/flame_01.png");
	RenderHelper::getInstance()->registerTexture(EXPLOSION, "Assets/Particle_Effects/star_01.png");
	RenderHelper::getInstance()->registerTexture(FIREWORK, "Assets/Particle_Effects/trace_01.png");
}

ParticleManager::~ParticleManager() {
	
}

void ParticleManager::init() {

}


/**
 * @brief Creates and adds a new particle to the manager.
 *
 * Adds a new particle with random properties within specified boundaries
 * to the particle list for rendering and updating.
 * @param x The x-coordinate where the particle is created.
 * @param y The y-coordinate where the particle is created.
 */
void ParticleManager::createParticle(float x, float y) {
	const float randSize = rand() % static_cast<int>(m_PARTICLE_MAX_WIDTH - m_PARTICLE_MIN_WIDTH) + m_PARTICLE_MIN_WIDTH;
	const float randDir = rand() % 360 / Math::m_PI * 180.f;

	float speed = rand() % static_cast<int>(m_MAX_PARTICLE_SPEED - m_MIN_PARTICLE_SPEED) + m_MIN_PARTICLE_SPEED;

	int sizeDiff = rand() % m_MAX_SIZE_DIFF;
	sizeDiff = rand() % 2 ? sizeDiff : -sizeDiff;

	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		speed *= 5;
	}

	const ParticleManager::Particle newParticle = ParticleManager::Particle{
		true,													// isactive
		{x, y},													// pos
		{randSize + sizeDiff, randSize + sizeDiff},				// initial size
		{randSize + sizeDiff, randSize + sizeDiff},				// size
		1.f,													// size multiplier
		{cosf(randDir), sinf(randDir)},							// normalized vector containing direction
		speed,													// speed of particle (scalar multiple to normalized dir vector)
		{0.7f, 1.f, 0.8f, 1},									// color 
		ParticleType::EXPLOSION,									// type
		1.f,													// lifetime
		1.f														// transparency
	};

	m_particles.push_back(newParticle);
}

/**
 * @brief Creates and adds a new explosion particle to the manager.
 *
 * Adds a new explosion particle with random properties within specified boundaries
 * to the particle list for rendering and updating.
 * @param x The x-coordinate where the particle is created.
 * @param y The y-coordinate where the particle is created.
 */
void ParticleManager::setParticlePos(float x, float y) {
	m_posX = x;
	m_posY = y;
}

/**
 * @brief Creates and adds a new explosion particle to the manager.
 *
 * Adds a new explosion particle with random properties within specified boundaries
 * to the particle list for rendering and updating.
 * @param x The x-coordinate where the particle is created.
 * @param y The y-coordinate where the particle is created.
 */
void ParticleManager::update(double dt)
{
	for (ParticleManager::Particle& p : m_particles) {
		switch (p.type) {
		case ParticleType::REGULAR:
			updateRegularParticle(p, dt);
			break;
		case ParticleType::EXPLOSION:
			updateExplosionParticle(p, dt);
			break;
		case ParticleType::FIREWORK:
			updateFireworkParticle(p, dt);
			break;
		}


		// update flag of inactive instances
		if (p.color.a <= 0 || p.size.x <= 0 || p.size.y <= 0) {
			p.m_isActive = false;
		}
	}

	std::vector<int> indexes;
	for (int i = 0; i < m_particles.size(); i++) {
		if (!m_particles[i].m_isActive) {
			indexes.push_back(i);
		}
	}

	std::sort(indexes.rbegin(), indexes.rend());

	for (const int i : indexes) {
		m_particles.erase(m_particles.begin() + i);
	}

	int toCreate = static_cast<int>(m_PARTICLES_CREATION_RATE / AEFrameRateControllerGetFrameRate());
	for (int i = 0; i < toCreate; i++) {
		createParticle(m_posX, m_posY);
	}


}

/**
 * @brief Renders all particles in the manager.
 *
 * Renders all active particles in the particle list using the render helper.
 */
void ParticleManager::render() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	for (const ParticleManager::Particle& p : m_particles) {
		const AEVec2 pos = stow(p.pos.x, p.pos.y);
		//RenderHelper::getInstance()->rect(pos.x, pos.y, p.size.x, p.size.y, 0, p.color, p.color.a);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef(p.type), 0, 0);
		AEGfxSetTransparency(p.transparency);
		AEGfxSetColorToMultiply(p.color.r, p.color.g, p.color.b, p.color.a);
		/*Create Transform data*/
		AEMtx33 scale, trans, rot, m_TransformData;
		AEMtx33Scale(&scale, p.size.x, p.size.y);
		AEMtx33Trans(&trans, pos.x, pos.y);
		AEMtx33Rot(&rot, 0);
		AEMtx33Concat(&m_TransformData, &scale, &rot);
		AEMtx33Concat(&m_TransformData, &trans, &m_TransformData);
		AEGfxSetTransform(m_TransformData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
}


void ParticleManager::createExplosionParticle(float x, float y) {
	const float randSize = rand() % static_cast<int>(m_PARTICLE_MAX_WIDTH - m_PARTICLE_MIN_WIDTH) + m_PARTICLE_MIN_WIDTH * 10;
	const float randAngle = rand() % 360 * Math::m_PI / 180.f;
	const float randDistance = static_cast<float>(rand() % m_MAX_POS_OFFSET);

	const ParticleManager::Particle newParticle = ParticleManager::Particle{
		true,
		{x + cosf(randAngle) * randDistance, y + sinf(randAngle) * randDistance},
		{randSize, randSize},
		{randSize, randSize},
		1.f,
		{cosf(randAngle), sinf(randAngle)},
		rand() % static_cast<int>(m_MAX_PARTICLE_SPEED - m_MIN_PARTICLE_SPEED) + m_MIN_PARTICLE_SPEED,
		{1.f, 0.5f, 0.f, 1},
		ParticleType::EXPLOSION
	};

	m_particles.push_back(newParticle);
}

void ParticleManager::createFireworkParticle(float x, float y, float explosionRadius) {
	const float randSize = rand() % static_cast<int>(m_PARTICLE_MAX_WIDTH - m_PARTICLE_MIN_WIDTH) + m_PARTICLE_MIN_WIDTH * 20;
	const float randAngle = rand() % 360 * Math::m_PI / 180.f;
	const float randRadius = static_cast<float>(rand() % static_cast<int>(explosionRadius));

	// Generate random color
	const float red = static_cast<float>(rand()) / RAND_MAX;
	const float green = static_cast<float>(rand()) / RAND_MAX;
	const float blue = static_cast<float>(rand()) / RAND_MAX;

	const ParticleManager::Particle newParticle = ParticleManager::Particle{
		true,
		{x + cosf(randAngle) * randRadius, y + sinf(randAngle) * randRadius},
		{1.f, randSize}, // Change width to 1.f for line-shaped particles
		{1.f, randSize},
		1.f,
		{cosf(randAngle), sinf(randAngle)},
		rand() % static_cast<int>(m_MAX_PARTICLE_SPEED - m_MIN_PARTICLE_SPEED) + m_MIN_PARTICLE_SPEED,
		{red, green, blue, 1.f}, // Set random color for the particle
		ParticleType::FIREWORK
	};

	m_particles.push_back(newParticle);
}

/**
 * @brief Updates the state of all particles in the manager.
 *
 * Updates each particle's position, size, and transparency,
 * and removes them if they are inactive.
 * @param dt Elapsed time since the last update, in seconds.
 */
void ParticleManager::updateRegularParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(m_OPACITY_LOSS / AEFrameRateControllerGetFrameRate());
	particle.color.r -= m_DARKEN_RATE;
	particle.color.g -= m_DARKEN_RATE;
	particle.color.b -= m_DARKEN_RATE;

	particle.sizeMultiplier -= static_cast<float>(m_SIZE_LOSS / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.vel.y += static_cast<float>(m_GRAVITY * dt);

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

void ParticleManager::updateExplosionParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(m_OPACITY_LOSS / AEFrameRateControllerGetFrameRate());

	particle.sizeMultiplier -= static_cast<float>(m_SIZE_LOSS / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

void ParticleManager::updateFireworkParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(m_OPACITY_LOSS / AEFrameRateControllerGetFrameRate());

	particle.sizeMultiplier -= static_cast<float>(m_SIZE_LOSS / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.vel.y += static_cast<float>(m_GRAVITY * dt);

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

