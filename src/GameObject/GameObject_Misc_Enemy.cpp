/* Start Header ************************************************************************/
/*!
\file GameObject_Misc_Enemy.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief misc enemy implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "GameObject_Misc_Enemy.h"
#include "GameObjectManager.h"


GameObject_Misc_Enemy::GameObject_Misc_Enemy() : 
	m_AttackPlayer{false},
	m_AttachedFloorTransform{nullptr},
	m_StartCombat{false},
	m_Transparency{1.0}
{
	m_Active = false;
	AEMtx33Identity(&m_TransformData);
}

GameObject_Misc_Enemy::~GameObject_Misc_Enemy()
{
}

void GameObject_Misc_Enemy::init()
{
}

void GameObject_Misc_Enemy::update(double _dt)
{
	if (m_Active && m_Health > 0)
	{
		updateGlobals();

		////////////////////////////////////////////////////////////////////////
		//Flickering
		if (m_FlickeringTimer > 0.0)
			m_Transparency = rand() % 2 ? 1.0f : -0.5f;
		else
			m_Transparency = 1.0f;
		m_FlickeringTimer -= _dt;

		/////////////////////////////////////////////////////////////////////////
		// Getting Hit
		{
			AEVec2 pos = wtos(m_LocalPos.x - camOffset.x, m_LocalPos.y - camOffset.y);
			//Check Collision
			if (AEInputCheckTriggered(AEVK_LBUTTON))
			{
				if (CollisionChecker::isMouseInRect(pos.x, pos.y, m_scale.x, m_scale.y, static_cast<float>(mouseX), static_cast<float>(mouseY))
					&& !m_AttackPlayer)
				{
					m_Health -= 1; //Just a simple subtraction
					m_FlickeringTimer = 0.3;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////
		// Movement
		{
			if (!m_AttackPlayer)
				UpdateEnemyMov(_dt);
			else
				UpdateCombatTransition(_dt);

			//Trigger combat if too close
			if (m_AttachedFloorTransform != nullptr)
			{
				//Easy check via scale of enemy if too close
				if (m_TransformData.m[0][0] >= 600 && !m_AttackPlayer) m_AttackPlayer = true;
				
			}
		}
	}
	else if (m_Active)
		m_StartCombat = 1;
}

void GameObject_Misc_Enemy::render()
{
	if (m_Active)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(m_Transparency);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("MISC_ENEMY_STRONG"), 0, 0);
		AEGfxSetTransform(m_TransformData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("MISC_ENEMYJAW_UPPER"), 0, 0);
		AEGfxSetTransform(m_TransformData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("MISC_ENEMYJAW_LOWER"), 0, 0);
		AEGfxSetTransform(m_TransformData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
}

void GameObject_Misc_Enemy::exit()
{
}

void GameObject_Misc_Enemy::ActivateEnemy(AEMtx33& m_transform, v_EnemyBehaviour _type)
{
	UNREFERENCED_PARAMETER(_type);
	m_Active = true;
	m_AttachedFloorTransform = &m_transform;
	AEMtx33Identity(&m_TransformData);
	m_AttackPlayer = false;
	m_StartCombat = false; // i know should put 0 but i like it this way since makes more sense
	m_scale.x = m_scale.y = 1.0f;
	m_Lifetime = 1.0;
	m_Health = rand() % 5 + 5;
	m_Transparency = 1.0;
	m_FlickeringTimer = 0.0;
	///////////////////////////////////////////
	// Not enough time to do
	//switch (_type)
	//{
	//case Normal:
	//case Hunt:
	//case Aggressive:
	//case Shy:
	//default:
	//	break;
	//}
}

void GameObject_Misc_Enemy::UpdateEnemyMov(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	//Reset Transform data
	AEMtx33Identity(&m_TransformData);

	//Scale with the tile
	AEMtx33ScaleApply(&m_TransformData, &m_TransformData, m_AttachedFloorTransform->m[0][0], m_AttachedFloorTransform->m[0][0]);

	//Translate with the tile
	m_TransformData.m[0][2] = m_AttachedFloorTransform->m[0][2];
	m_TransformData.m[1][2] = m_AttachedFloorTransform->m[1][2];

	m_scale.x = m_TransformData.m[0][0];
	m_scale.y = m_TransformData.m[1][1];
	m_LocalPos.x = m_TransformData.m[0][2] / m_scale.x;
	m_LocalPos.y = m_TransformData.m[1][2] / (m_scale.y * 2);

}

void GameObject_Misc_Enemy::UpdateCombatTransition(double _dt)
{
    ////////////////////////////////////////////////////////////////////////
    //Animation Steps (I lazy animate mouth)
	{
		static double m_Speed = 0.3;
		m_Lifetime -= _dt + m_Speed / 10;
		if (m_Lifetime >= 0.0)
		{
			f32 camX, camY;
			AEGfxGetCamPosition(&camX, &camY);
			camY += -AEGfxGetWindowHeight() / 2 + 200;
			camY *= -1;

			m_LocalPos.x = lerp(m_LocalPos.x, camX, static_cast<float>(_dt / (m_Lifetime * 1000)));
			m_LocalPos.y = lerp(m_LocalPos.y, camY, static_cast<float>(_dt / (m_Lifetime * 1000)));

			// Move in the positive direction if the current position is less than the target position
			if (m_LocalPos.x > camX)
				m_LocalPos.x += static_cast<float>(_dt / (m_Lifetime * 1000));

			// Move in the negative direction if the current position is greater than the target position
			if (m_LocalPos.y > camY)
				m_LocalPos.y -= static_cast<float>(_dt / (m_Lifetime * 1000));

			m_scale.x += lerp(m_scale.x, 3.f, static_cast<float>(_dt / (m_Lifetime * 1000))) * static_cast<float>(m_Speed);
			m_scale.y += lerp(m_scale.y, 3.f, static_cast<float>(_dt / (m_Lifetime * 1000))) * static_cast<float>(m_Speed);
		}
		else
		{
			m_StartCombat = 2;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//Transform Update (DONT CHANGE)
	{
		AEMtx33Identity(&m_TransformData);
		AEMtx33ScaleApply(&m_TransformData, &m_TransformData, m_scale.x, m_scale.y);
		AEMtx33TransApply(&m_TransformData, &m_TransformData, m_LocalPos.x, m_LocalPos.y);
	}

}

GameObject_Misc_Enemy* Create::miscEnemy(const AEVec2& _position, const AEVec2& _scale)
{
	GameObject_Misc_Enemy* result = new GameObject_Misc_Enemy();
	result->m_LocalPos = _position;
	result->m_scale = _scale;
	result->m_Active = false;
	result->m_RefName = "MiscEnemy";
	result->init();
	//result->bool m_bCollider(false);
	GameObjectManager::getInstance()->addEntity(result);
	return result;
}
