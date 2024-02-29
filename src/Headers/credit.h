#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"


class SceneManager;
class SceneCredits : public Scene
{
public:
	SceneCredits();
	~SceneCredits();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	s8 pFont;

	f32 textPosX, textPosY;

	struct Point
	{
		float x, y;
	};

	struct {
		AEGfxVertexList* mesh;
		AEGfxTexture* bg;
		s8 fontID;
	}credits;


	void texture(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency);
	//{


	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); // texture mode
	//	AEGfxTextureSet(texture, 0, 0); // set the "image" (texture)

	//	// Create a scale matrix
	//	AEMtx33 scale = { 0 };
	//	AEMtx33Scale(&scale, scaleX, scaleY);

	//	// Create a rotation matrix
	//	AEMtx33 rotate = { 0 };
	//	AEMtx33Rot(&rotate, rotation);

	//	// Create a translation matrix
	//	AEMtx33 translate = { 0 };
	//	AEMtx33Trans(&translate, positionX, positionY);

	//	// Concat the matrices (TRS)
	//	AEMtx33 transform = { 0 };
	//	AEMtx33Concat(&transform, &translate, &rotate);
	//	AEMtx33Concat(&transform, &transform, &scale);

	//	// Choose the transform to use
	//	AEGfxSetTransform(transform.m);

	//	// Change the transparency
	//	AEGfxSetTransparency(transparency);

	//	// Actually drawing the mesh
	//	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);



	//}


private:

	SceneCredits(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneCredits* sInstance; // The pointer to the object that gets registered

};
