#include "pch.h"
#include "ParticleManager.h"
#include "MyMath.h"


ParticleManager* ParticleManager::instance = nullptr;

ParticleManager* ParticleManager::GetInstance() {
	if (instance == nullptr) {
		instance = new ParticleManager();
	}
	return instance;
}

ParticleManager::ParticleManager() {
	particles.reserve(PROJECTED_MAX_PARTICLES);
}

ParticleManager::~ParticleManager() {

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
		{0.25f, 1.f, 0.25f, 1},									// color 
	};

	particles.push_back(newParticle);
}

void ParticleManager::setParticlePos(float x, float y) {
	posX = x;
	posY = y;
}

void ParticleManager::init() {

}

void ParticleManager::update(double dt) {
	for (ParticleManager::Particle& p : particles) {
		// update flag of inactive instances
		if (p.color.a <= 0 || p.size.x <= 0 || p.size.y <= 0)
		{
			p.isActive = false;
		}

		// update opacity
		p.color.a -= static_cast<float>(opacityLoss / AEFrameRateControllerGetFrameRate());
		p.color.r -= darkenRate;
		p.color.g -= darkenRate;
		p.color.b -= darkenRate;

		// update size
		p.sizeMultiplier -= static_cast<float>(sizeLoss / AEFrameRateControllerGetFrameRate());
		p.size.x = p.initialSize.x * p.sizeMultiplier;
		p.size.y = p.initialSize.y * p.sizeMultiplier;

		// apply gravity
		p.vel.y += static_cast<float>(GRAVITY * dt);

		// update position
		p.pos.x += p.vel.x * p.speed * static_cast<float>(dt);
		p.pos.y += p.vel.y * p.speed * static_cast<float>(dt);
	}

	// vector of indexes of inactive particles
	std::vector<int> indexes{};
	// remove inactive particles
	for (int i{}; i < particles.size(); i++) {
		if (!particles[i].isActive) {
			indexes.push_back(i);
		}
	}

	// sort indexes in descending order so erasing doesnt go out of range
	std::sort(indexes.rbegin(), indexes.rend());

	for (const int i : indexes) {
		particles.erase(particles.begin() + i);
	}


	// create particles
	int toCreate = static_cast<int>(particlesCreationRate / AEFrameRateControllerGetFrameRate());
	for (int i{}; i < toCreate; i++) {
		createParticle(posX, posY);
	}
}


void ParticleManager::render() {
	for (const ParticleManager::Particle& p : particles) {
		const Point pos = stow(p.pos.x, p.pos.y);
		RenderHelper::getInstance()->rect(pos.x, pos.y, p.size.x, p.size.y, 0, p.color, p.color.a);
	}
}
