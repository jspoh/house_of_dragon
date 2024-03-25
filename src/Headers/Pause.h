#pragma once

#include "Pch.h"


class Pause {
private:
	Pause();

	static constexpr u8 PAUSE_KEY = AEVK_P;

public:
	static Pause& getInstance();

	bool isPaused;

	~Pause() = default;

	void init();

	void update(double dt);

	void render();
};

