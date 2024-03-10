#pragma once

class ParticleManager : public Singleton<ParticleManager> {
public:
	~ParticleManager() override;

	//static ParticleManager* GetInstance();

	void init();

	void update(double dt);

	void render();

	/**
	 * set particle spawn position.
	 * 
	 * \param x
	 * \param y
	 */
	void setParticlePos(float x, float y);

	struct Particle {
		bool isActive;
		AEVec2 pos;
		AEVec2 initialSize;
		AEVec2 size;
		float sizeMultiplier;
		AEVec2 vel;
		float speed;
		Color color;
	};

	// YOU SHOULDNT BE USING THIS!! ONLY FOR SINGLETON TEMPLATE!!
	ParticleManager();

private:
	//static ParticleManager* instance;

	static constexpr int PROJECTED_MAX_PARTICLES = 2048;
	static constexpr int particlesCreationRate = 120;		// number of particles to create per second
	static constexpr float opacityLoss = 1.f;				// 0 to 1, represents percentage opacity lost per second
	static constexpr float sizeLoss = 1.f;					// percentage of size lost in 1 second. 1 means will lose 100% of size in 1 sec
	static constexpr float minParticleSpeed = 50.f;
	static constexpr float maxParticleSpeed = 100.f;
	static constexpr float darkenRate = 0.01f;				// rate which the particles darken
	static constexpr int maxPosOffset = 20;				// max positional offset when creating particle
	static constexpr int maxSizeDiff = 10;			// max size difference
	static constexpr float PARTICLE_MIN_WIDTH = 3.f;
	static constexpr float PARTICLE_MAX_WIDTH = 5.f;
	// assuming particle is square-ish..?? not rly
	static constexpr float PARTICLE_MIN_HEIGHT = PARTICLE_MIN_WIDTH;
	static constexpr float PARTICLE_MAX_HEIGHT = PARTICLE_MAX_WIDTH;
	static constexpr float GRAVITY = 5.f;

	std::vector<Particle> particles{ 0 };

	// particle spawn locations, use setParticlePos to set
	float posX = 0;
	float posY = 0;



	void createParticle(float posX, float posY);
};
