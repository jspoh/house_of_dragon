/* Start Header ************************************************************************/
/*!
\file SceneCredit.h
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief m_credits scene
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

/**
 * @class SceneCredits
 * @brief Handles the credit scene functionality.
 *
 * This class is responsible for handling the operations required for the
 * credit scene including loading assets, initializing fonts, updating
 * scrolling text, rendering scene contents, and cleaning up resources.
 */

class SceneCredits : public Scene
{
public:
	SceneCredits();
	SceneCredits(SceneManager* _sceneMgr); // This is used to register to SceneManager
	~SceneCredits();

	virtual void load();
	virtual void init();
	virtual void update(double dt);
	virtual void render();
	virtual void exit();
	s8 m_pFontL;
	s8 m_pFontM;
	s8 m_pFontS;
	s8 m_pFontxS;


	f32 m_textPosX, m_textPosY;

	struct Point
	{
		float x, y;
	};

	struct Credits {
		AEGfxVertexList* mesh;
		AEGfxTexture* bg;
		s8 fontID;
	};

	Credits m_credits{ 0 };

	void texture(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency);
	

private:

	float m_elapsedTime;
	static SceneCredits* m_sInstance; 

};
