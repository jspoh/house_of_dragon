#include "Pch.h"
#include "Pause.h"


Pause::Pause() : isPaused{ false } {

}

Pause& Pause::getInstance() {
	static Pause instance;
	return instance;
}


void Pause::init() {

}


void Pause::update(double dt) {
	if (AEInputCheckTriggered(PAUSE_KEY)) {
		isPaused = !isPaused;
	}
}

void Pause::render() {
	if (!isPaused) {
		return;
	}
	RenderHelper::getInstance()->rect("invis", 0, 0, 9999.f, 9999.f, 0, Color{ 0,0,0,0.7f }, 0.7f);
}
