#include "Pch.h"
#include "SceneSetting.h"
#include "ParticleManager.h"

// should put in namespace to avoid cluttering
//namespace {
	//static char strBuffer[1024];
	//static char strBuffer1[1024];
	//static float speed = 8;
	//static Point text;
	//static float scrolling;
	//static float timef;
//}




SceneSetting* SceneSetting::sInstance = new SceneSetting(SceneManager::GetInstance());

SceneSetting::SceneSetting(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneSetting", this);
	//SoundManager::GetInstance()->getVolume(sfxVolume, musicVolume);		// tf?? calling this stops sound from working


}

SceneSetting::~SceneSetting()
{

}



void SceneSetting::Load()
{

	RenderHelper::getInstance()->registerTexture("settingbg", "./Assets/Menu/setting.png");
	RenderHelper::getInstance()->registerTexture("musicnote", "./Assets/Menu/musicnote.png");

}


void SceneSetting::Init()
{
	ParticleManager::GetInstance()->init();



}


void SceneSetting::Update(double dt)
{
	//if "Escape" button triggered, go to menu state
	if (AEInputCheckTriggered(AEVK_Q))
		SceneManager::GetInstance()->SetActiveScene("SceneMenu");

	int mX, mY;
	AEInputGetCursorPosition(&mX, &mY);

	ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mX), static_cast<float>(mY));
	ParticleManager::GetInstance()->update(dt);

	/* update slider position */

	// get updated volume values
	SoundManager::GetInstance()->getVolume(sfxVolume, musicVolume);

	const float sliderRadius = max(sliderScale.x, sliderScale.y);

	const float minX = soundBarPos.x - soundBarScale.x / 2.f;
	const float maxX = soundBarPos.x + soundBarScale.x / 2.f;

	soundSliderPos.x = minX + sfxVolume * soundBarScale.x;
	musicSliderPos.x = minX + musicVolume * soundBarScale.x;
}

void SceneSetting::Render()
{
	//texture1(mySetting.bg, 0.f, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), 0.f, 0.f, mySetting.mesh, 1.f);
	RenderHelper::getInstance()->texture("settingbg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));
	//RenderHelper::getInstance()->texture("settingbg", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, mySetting.mesh, 1.0f);
	RenderHelper::getInstance()->text("DIFFICULTY	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 3.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	RenderHelper::getInstance()->text("SOUND	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 2.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	RenderHelper::getInstance()->text("MUSIC	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 1.6f, 1.0f, 1.0f, 1.0f, 1.0f);

	RenderHelper::getInstance()->rect(soundBarPos.x, soundBarPos.y, soundBarScale.x, soundBarScale.y, 0, soundBarColor);
	RenderHelper::getInstance()->rect(musicBarPos.x, musicBarPos.y, soundBarScale.x, soundBarScale.y, 0, soundBarColor);
	RenderHelper::getInstance()->texture("musicnote", soundSliderPos.x, soundSliderPos.y, sliderScale.x, sliderScale.y);
	RenderHelper::getInstance()->texture("musicnote", musicSliderPos.x, musicSliderPos.y, sliderScale.x, sliderScale.y);


	ParticleManager::GetInstance()->render();

}

void SceneSetting::Exit()
{
	//AEGfxMeshFree(mySetting.mesh);

}



//void SceneSetting::texture1(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency)
//{
//	UNREFERENCED_PARAMETER(transparency);
//	UNREFERENCED_PARAMETER(texture);
//	UNREFERENCED_PARAMETER(scaleY);
//	UNREFERENCED_PARAMETER(scaleX);
//	UNREFERENCED_PARAMETER(rotation);
//	UNREFERENCED_PARAMETER(positionX);
//	UNREFERENCED_PARAMETER(positionY);
//	UNREFERENCED_PARAMETER(mesh);
//	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
//	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//
//
//	AEMtx33 scale = { 0 };
//	AEMtx33Scale(&scale, 1200, 750);
//
//	AEMtx33 transform;
//	AEMtx33Trans(&transform, 0, 0);
//
//	AEMtx33 model = { 0 };
//	AEMtx33Concat(&model, &scale, &transform);
//
//	 prepare to draw
//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
//	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
//	AEGfxSetColorToAdd(0, 0, 0, 1);
//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
//	AEGfxSetTransparency(1);
//	AEGfxSetTransform(model.m);
//	AEGfxTextureSet(mySetting.bg, 0, 0);
//	AEGfxMeshDraw(mySetting.mesh, AE_GFX_MDM_TRIANGLES);
//
//
//
//
//}