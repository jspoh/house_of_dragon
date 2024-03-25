#pragma once

#include "Pch.h"


class Pause {
private:
	Pause();

	static constexpr u8 PAUSE_KEY = AEVK_P;

	static constexpr int NUM_BTNS = 3;
	std::array<std::pair<std::string, float>, NUM_BTNS> btns{
		std::pair{std::make_pair("RESUME", 1)},
		{std::make_pair("RESTART", 1)},
		{std::make_pair("EXIT", 1)}
	};

	static constexpr int BTN_WIDTH = 250;
	static constexpr int BTN_HEIGHT = 100;
	static constexpr int BTN_Y_GAP = 50;
	static constexpr float HOVER_BTN_SCALE = 1.1f;

	AEVec2 camOffset;

public:
	static Pause& getInstance();

	bool isPaused;

	~Pause() = default;

	void init();

	void update(double dt);

	void render() const;
};

