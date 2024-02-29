#include "credit.h"


static char strBuffer[1024];
static float speed;
static float lineSpace;
static float gt;
static float timef;
static float scrollingY;
static float dt;
static Point text;




SceneCredits* SceneCredits::sInstance = new SceneCredits(SceneManager::GetInstance());

SceneCredits::SceneCredits()
{

}

SceneCredits::SceneCredits(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneCredits", this);
}

SceneCredits::~SceneCredits()
{
}



void SceneCredits::Load()
{
	credits.bg = AEGfxTextureLoad("Assets/bg.jpg");
	//credits.mesh = 0; 
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	credits.mesh = AEGfxMeshEnd();
}


void SceneCredits::Init()
{

	//init for scrolling scene
	speed = 30;  //scrolling speed
	text = { -0.25,0 };  //coordinate of textes
	scrollingY = text.y;  //coordinate of scrolling Y
	lineSpace = 15;  //space between every line
	gt = 1;

	pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 40);

	//////////////////////////////////////////////////////

	f32 TextWidth, TextHeight = 0;

	sprintf_s(strBuffer, "'Esc' to exit");
	AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);

	textPosX = -TextWidth / 2;
	textPosY = -TextHeight / 2;
}


void SceneCredits::Update(double dt)
{
	//if left button clicked, scrolling speed = 10
	if (AEInputCheckCurr(AEVK_LBUTTON)) {
		gt = 10;
	}
	//else, scrolling speed = 1;
	else {
		gt = 1;
	}

	//time = dt * speed;
	timef = dt * gt;

	//if "Escape" button triggered, go to menu state
	if (AEInputCheckTriggered(AEVK_ESCAPE))
		SceneManager::GetInstance()->SetActiveScene("SceneMenu");

}

void SceneCredits::Render()
{

	//AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);


	//AEMtx33 scale = { 0 };
	//AEMtx33Scale(&scale, 5670, 3780);

	//AEMtx33 transform;
	//AEMtx33Trans(&transform, 0, 0);

	//AEMtx33 model = { 0 };
	//AEMtx33Concat(&model, &scale, &transform);

	//// prepare to draw
	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	//AEGfxSetColorToAdd(0, 0, 0, 1);
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxSetTransparency(1);
	//AEGfxSetTransform(model.m);
	//AEGfxTextureSet(credits.bg, 0, 0);
	//AEGfxMeshDraw(credits.mesh, AE_GFX_MDM_TRIANGLES);
	int i = -70;

	//position y will change according to time * speed;
	scrollingY += timef * speed;

	texture(credits.bg, 0.f, 1200.f, 960.f, 0.f, 0.f, credits.mesh, 1);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);




	AEGfxPrint(pFont, strBuffer, textPosX, textPosY, 1.f, 1.f, 1.f, 1.f, 1.0f);

	textPosY -= speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;

	//sprintf_s(strBuffer, "'Esc' to exit");
	//AEGfxPrint(pFont, strBuffer, -0.9f, 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	//AEGfxGetPrintSize(pFont, strBuffer, 1.0f, &TextWidth, &TextHeight);
	//+i;



}

void SceneCredits::Exit()
{
	AEGfxMeshFree(credits.mesh);
}
void SceneCredits::texture(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency)
{


	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); // texture mode
	//AEGfxTextureSet(texture, 0, 0); // set the "image" (texture)
	//
	//// Create a scale matrix
	//AEMtx33 scale = { 0 };
	//AEMtx33Scale(&scale, scaleX, scaleY);

	//// Create a rotation matrix
	//AEMtx33 rotate = { 0 };
	//AEMtx33Rot(&rotate, rotation);

	//// Create a translation matrix
	//AEMtx33 translate = { 0 };
	//AEMtx33Trans(&translate, positionX, positionY);

	//// Concat the matrices (TRS)
	//AEMtx33 transform = { 0 };
	//AEMtx33Concat(&transform, &translate, &rotate);
	//AEMtx33Concat(&transform, &transform, &scale);

	//// Choose the transform to use
	//AEGfxSetTransform(transform.m);

	//// Change the transparency
	//AEGfxSetTransparency(transparency);

	//// Actually drawing the mesh
	//AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);


	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 5670, 3780);

	AEMtx33 transform;
	AEMtx33Trans(&transform, 0, 0);

	AEMtx33 model = { 0 };
	AEMtx33Concat(&model, &scale, &transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0, 0, 0, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);
	AEGfxSetTransform(model.m);
	AEGfxTextureSet(credits.bg, 0, 0);
	AEGfxMeshDraw(credits.mesh, AE_GFX_MDM_TRIANGLES);




}