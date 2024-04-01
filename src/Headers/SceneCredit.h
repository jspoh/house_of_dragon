/* Start Header ************************************************************************/
/*!
\file SceneCredit.h
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief credits scene
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"


class SceneCredits : public Scene
{
public:
	SceneCredits();
	SceneCredits(SceneManager* _sceneMgr); // This is used to register to SceneManager
	~SceneCredits();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	s8 pFontL;
	s8 pFontM;
	s8 pFontS;
	s8 pFontxS;

	f32 textPosX, textPosY;

	struct Point
	{
		float x, y;
	};

	struct Credits {
		AEGfxVertexList* mesh;
		AEGfxTexture* bg;
		s8 fontID;
	};

	Credits credits{ 0 };


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

	//SceneCredits(SceneManager* _sceneMgr); // This is used to register to SceneManager
	float m_elapsedTime;
	static SceneCredits* sInstance; // The pointer to the object that gets registered

};
