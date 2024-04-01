/* Start Header ************************************************************************/
/*!
\file ParticleManager.cpp
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 01 Apr 2024
\brief particle manager implementation
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



ParticleManager::ParticleManager() {
	particles.reserve(PROJECTED_MAX_PARTICLES);

	/*********************************************
	//PARTICLE TEXTURES REGISTRATION
	**********************************************/
	RenderHelper::getInstance()->registerTexture(REGULAR, "Assets/Particle_Effects/Single Particles/PNG (Transparent)/flame_01.png");
	RenderHelper::getInstance()->registerTexture(EXPLOSION, "Assets/Particle_Effects/Single Particles/PNG (Transparent)/star_01.png");
	RenderHelper::getInstance()->registerTexture(FIREWORK, "Assets/Particle_Effects/Single Particles/PNG (Transparent)/trace_01.png");
}

ParticleManager::~ParticleManager() {
	RenderHelper::getInstance()->removeTextureByRef(REGULAR);
	RenderHelper::getInstance()->removeTextureByRef(EXPLOSION);
	RenderHelper::getInstance()->removeTextureByRef(FIREWORK);
}

void ParticleManager::init() {

}



void ParticleManager::createParticle(float x, float y) {
	const float randSize = rand() % static_cast<int>(PARTICLE_MAX_WIDTH - PARTICLE_MIN_WIDTH) + PARTICLE_MIN_WIDTH;
	const float randDir = rand() % 360 / Math::m_PI * 180.f;

	float speed = rand() % static_cast<int>(maxParticleSpeed - minParticleSpeed) + minParticleSpeed;

	int sizeDiff = rand() % maxSizeDiff;
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

	particles.push_back(newParticle);
}

void ParticleManager::setParticlePos(float x, float y) {
	posX = x;
	posY = y;
}

void ParticleManager::update(double dt)
{
	for (ParticleManager::Particle& p : particles) {
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
			p.isActive = false;
		}
	}

	std::vector<int> indexes;
	for (int i = 0; i < particles.size(); i++) {
		if (!particles[i].isActive) {
			indexes.push_back(i);
		}
	}

	std::sort(indexes.rbegin(), indexes.rend());

	for (const int i : indexes) {
		particles.erase(particles.begin() + i);
	}

	int toCreate = static_cast<int>(particlesCreationRate / AEFrameRateControllerGetFrameRate());
	for (int i = 0; i < toCreate; i++) {
		createParticle(posX, posY);
	}


}


void ParticleManager::render() {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	for (const ParticleManager::Particle& p : particles) {
		const Point pos = stow(p.pos.x, p.pos.y);
		//RenderHelper::getInstance()->rect(pos.x, pos.y, p.size.x, p.size.y, 0, p.color, p.color.a);
		AEGfxTextureSet(RenderHelper::getInstance()->GetTexture(p.type), 0, 0);
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
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
}


void ParticleManager::createExplosionParticle(float x, float y) {
	const float randSize = rand() % static_cast<int>(PARTICLE_MAX_WIDTH - PARTICLE_MIN_WIDTH) + PARTICLE_MIN_WIDTH * 10;
	const float randAngle = rand() % 360 * Math::m_PI / 180.f;
	const float randDistance = static_cast<float>(rand() % maxPosOffset);

	const ParticleManager::Particle newParticle = ParticleManager::Particle{
		true,
		{x + cosf(randAngle) * randDistance, y + sinf(randAngle) * randDistance},
		{randSize, randSize},
		{randSize, randSize},
		1.f,
		{cosf(randAngle), sinf(randAngle)},
		rand() % static_cast<int>(maxParticleSpeed - minParticleSpeed) + minParticleSpeed,
		{1.f, 0.5f, 0.f, 1},
		ParticleType::EXPLOSION
	};

	particles.push_back(newParticle);
}

void ParticleManager::createFireworkParticle(float x, float y, float explosionRadius) {
	const float randSize = rand() % static_cast<int>(PARTICLE_MAX_WIDTH - PARTICLE_MIN_WIDTH) + PARTICLE_MIN_WIDTH * 20;
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
		rand() % static_cast<int>(maxParticleSpeed - minParticleSpeed) + minParticleSpeed,
		{red, green, blue, 1.f}, // Set random color for the particle
		ParticleType::FIREWORK
	};

	particles.push_back(newParticle);
}


void ParticleManager::updateRegularParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(opacityLoss / AEFrameRateControllerGetFrameRate());
	particle.color.r -= darkenRate;
	particle.color.g -= darkenRate;
	particle.color.b -= darkenRate;

	particle.sizeMultiplier -= static_cast<float>(sizeLoss / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.vel.y += static_cast<float>(GRAVITY * dt);

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

void ParticleManager::updateExplosionParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(opacityLoss / AEFrameRateControllerGetFrameRate());

	particle.sizeMultiplier -= static_cast<float>(sizeLoss / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

void ParticleManager::updateFireworkParticle(Particle& particle, double dt) {
	particle.color.a -= static_cast<float>(opacityLoss / AEFrameRateControllerGetFrameRate());

	particle.sizeMultiplier -= static_cast<float>(sizeLoss / AEFrameRateControllerGetFrameRate());
	particle.size.x = particle.initialSize.x * particle.sizeMultiplier;
	particle.size.y = particle.initialSize.y * particle.sizeMultiplier;

	particle.vel.y += static_cast<float>(GRAVITY * dt);

	particle.pos.x += particle.vel.x * particle.speed * static_cast<float>(dt);
	particle.pos.y += particle.vel.y * particle.speed * static_cast<float>(dt);
}

