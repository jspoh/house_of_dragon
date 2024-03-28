#include "Pch.h"
#include "Pause.h"
#include "SceneManager.h"


Pause::Pause() : isPaused{ false } {
	camOffset = { 0, 0 };
}

Pause& Pause::getInstance() {
	static Pause instance;
	return instance;
}


void Pause::init() {

}


void Pause::update([[maybe_unused]] double dt) {
	if (AEInputCheckTriggered(PAUSE_KEY)) {
		isPaused = !isPaused;
	}
	AEGfxGetCamPosition(&camOffset.x, &camOffset.y);

	if (!isPaused) {
		return;
	}

	int mX, mY;
	AEInputGetCursorPosition(&mX, &mY);

	// check for clicks
	constexpr int yOffset = BTN_HEIGHT + BTN_Y_GAP;
	constexpr int btnYStart = 0 + (NUM_BTNS * yOffset - BTN_Y_GAP) / 2;
	int btnY = btnYStart;

	for (auto& [b, scale] : btns) {
		Point pos = wtos(0, static_cast<float>(btnY));
		if (CollisionChecker::isMouseInRect(pos.x, pos.y, BTN_WIDTH * scale, BTN_HEIGHT * scale, static_cast<float>(mX), static_cast<float>(mY))) {
			
			scale = HOVER_BTN_SCALE;

			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				if (b == "RESUME") {
					isPaused = false;
				}
				else if (b == "RESTART") {
					// !TODO: call state or get scenelevelbuilder to restart combatscene

				}
				else if (b == "EXIT") {
					isPaused = false;
					AEGfxSetCamPosition(0, 0);
					SceneManager::GetInstance()->SetActiveScene("SceneMenu");
				}
			}
		}
		else {
			scale = 1.f;
		}
		btnY -= yOffset;
	}

}

void Pause::render() const {
	if (!isPaused) {
		return;
	}
	RenderHelper::getInstance()->rect("invis", 0, 0, 9999.f, 9999.f, 0, Color{ 0,0,0,0.7f }, 0.7f);

	constexpr int yOffset = BTN_HEIGHT + BTN_Y_GAP;
	constexpr int btnYStart = 0 + (NUM_BTNS * yOffset - BTN_Y_GAP) / 2;
	int btnY = btnYStart;

	for (const auto& [b, scale] : btns) {
		RenderHelper::getInstance()->texture("button", 0 + camOffset.x, btnY + camOffset.y, scale * BTN_WIDTH, scale * BTN_HEIGHT);
		RenderHelper::getInstance()->text(b, AEGfxGetWindowWidth() / 2.f, -btnY + AEGfxGetWindowHeight() / 2.f);
		btnY -= yOffset;
	}
}
