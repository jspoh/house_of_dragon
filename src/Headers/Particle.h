#pragma once


struct Particle {
	bool isActive;
	AEVec2 pos;
	AEVec2 initialSize;
	AEVec2 size;
	float sizeMultiplier;
	AEVec2 vel;
	float speed;		// scalar for velocity (velocity represents direction as unit vector, scalar represents magnitude aka speed)
	Color color;		// between 0 and 1 (percentage)
};
