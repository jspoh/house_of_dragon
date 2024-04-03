/* Start Header ************************************************************************/
/*!
\file GameObject_Projectiles.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief projectiles game object implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "GameObject_Projectiles.h"
#include "GameObjectManager.h"


GameObject_Projectiles::GameObject_Projectiles() :
	m_type { P_FireBall_Red },
	m_Speed {0.2f}
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, FireBall_sprite_uv_height, // bottom left
		0.5f, -0.5f, 0xFFFFFFFF, FireBall_sprite_uv_width, FireBall_sprite_uv_height, // bottom right 
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top left

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, FireBall_sprite_uv_width, FireBall_sprite_uv_height, // bottom right 
		0.5f, 0.5f, 0xFFFFFFFF, FireBall_sprite_uv_width, 0.0f,   // top right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // bottom left
	// Saving the mesh (list of triangles) in pMesh
	pFireBallMesh = AEGfxMeshEnd();


	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, EnergyBall_sprite_uv_height, // bottom left
		0.5f, -0.5f, 0xFFFFFFFF, EnergyBall_sprite_uv_width, EnergyBall_sprite_uv_height, // bottom right 
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top left

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, EnergyBall_sprite_uv_width, EnergyBall_sprite_uv_height, // bottom right 
		0.5f, 0.5f, 0xFFFFFFFF, EnergyBall_sprite_uv_width, 0.0f,   // top right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // bottom left
	pEnergyBallMesh = AEGfxMeshEnd();
}

GameObject_Projectiles::~GameObject_Projectiles()
{
}

void GameObject_Projectiles::Init()
{
}

void GameObject_Projectiles::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	if (m_Active)
	{
		//Movement Logic
		m_Lifetime -= _dt + m_Speed/10;
		if (m_Lifetime >= 0.0)
		{
			m_LocalPos.x = lerp(m_LocalPos.x, static_cast<float>(PLAYERSCREENPOSX), static_cast<float>(_dt / (m_Lifetime * 1000.0)));
			m_LocalPos.y = lerp(m_LocalPos.y, static_cast<float>(PLAYERSCREENPOSY), static_cast<float>(_dt / (m_Lifetime * 1000.0)));

			// Move in the positive direction if the current position is less than the target position
			if (m_LocalPos.x > PLAYERSCREENPOSX)
				m_LocalPos.x += static_cast<float>(_dt / (m_Lifetime * 1000));

			// Move in the negative direction if the current position is greater than the target position
			if (m_LocalPos.y > PLAYERSCREENPOSY)
				m_LocalPos.y -= static_cast<float>(_dt / (m_Lifetime * 1000));

			m_Scale.x += lerp(m_Scale.x, 400.f, static_cast<float>(_dt / (m_Lifetime * 1000.0))) * static_cast<float>(m_Speed);
			m_Scale.y += lerp(m_Scale.y, 400.f, static_cast<float>(_dt / (m_Lifetime * 1000.0))) * static_cast<float>(m_Speed);
		}
		else
		{
			m_Active = false;
			//Send message to deal damage
		}

		// Update the animation timer.
		// animation_timer should go up to animation_duration_per_frame.
		animation_timer += static_cast<float>(_dt);
		if (animation_timer >= animation_duration_per_frame)
		{
			// Reset the timer.
			animation_timer = 0;
			u32 current_sprite_row, current_sprite_col;
			switch (m_type)
			{
			case P_FireBall_Red:
			case P_FireBall_Blue:
			case P_FireBall_Green:
				current_sprite_index = ++current_sprite_index % FireBall_spritesheet_max_sprites;

				current_sprite_row = current_sprite_index / FireBall_spritesheet_cols;
				current_sprite_col = current_sprite_index % FireBall_spritesheet_cols;

				current_sprite_uv_offset_x = FireBall_sprite_uv_width * current_sprite_col;
				current_sprite_uv_offset_y = FireBall_sprite_uv_height * current_sprite_row;
				break;
			case P_EnergyBall_Blue:
			case P_EnergyBall_Dark:
			case P_EnergyBall_Green:
				current_sprite_index = ++current_sprite_index % EnergyBall_spritesheet_max_sprites;

				current_sprite_row = current_sprite_index / EnergyBall_spritesheet_cols;
				current_sprite_col = current_sprite_index % EnergyBall_spritesheet_cols;

				current_sprite_uv_offset_x = EnergyBall_sprite_uv_width * current_sprite_col;
				current_sprite_uv_offset_y = EnergyBall_sprite_uv_height * current_sprite_row;
				break;
			}
		}
	}
}

void GameObject_Projectiles::Render()
{
	if (m_Active)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Set the texture to pTex
		AEMtx33 transform = { 0 };
		AEMtx33 scale;
		AEMtx33Scale(&scale, m_Scale.x, m_Scale.y);
		AEMtx33 trans;
		AEMtx33Trans(&trans, m_LocalPos.x, m_LocalPos.y);
		AEMtx33 rotate;
		AEMtx33Rot(&rotate, 0);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &trans, &transform);
		AEGfxSetTransform(transform.m);
		switch (m_type)
		{
		case P_FireBall_Red:
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			goto RENDERFIREBALL; break;
		case P_FireBall_Blue:
			AEGfxSetColorToMultiply(0.0f, 0.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			goto RENDERFIREBALL; break;
		case P_FireBall_Green:
			AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		RENDERFIREBALL:
			AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FIREBALL"), current_sprite_uv_offset_x, current_sprite_uv_offset_y);
			AEGfxMeshDraw(pFireBallMesh, AE_GFX_MDM_TRIANGLES);
			break;
		case P_EnergyBall_Blue: goto RENDERENERGY; break;
		case P_EnergyBall_Dark: goto RENDERENERGY; break;
		case P_EnergyBall_Green:
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		RENDERENERGY:
			AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("ENERGYBALL"), current_sprite_uv_offset_x, current_sprite_uv_offset_y);
			AEGfxMeshDraw(pEnergyBallMesh, AE_GFX_MDM_TRIANGLES);
			break;
		}

		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	}


}

void GameObject_Projectiles::Exit()
{
	AEGfxMeshFree(pFireBallMesh);
	AEGfxMeshFree(pEnergyBallMesh);
}

void GameObject_Projectiles::FireAtPlayer(const AEVec2& _startpos, 
	const AEVec2& _startscale, 
	GameObject_Projectiles::ProjectileType _type,
	double _speed,
	double _MovTimer)
{
	m_LocalPos = _startpos;
	m_Scale = _startscale;
	m_Lifetime = _MovTimer;
	m_Speed = _speed;
	m_type = _type;
	m_Active = true;
}

GameObject_Projectiles* Create::Projectiles(const AEVec2& _position, const AEVec2& _scale, GameObject_Projectiles::ProjectileType _type)
{
	GameObject_Projectiles* result = new GameObject_Projectiles();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	result->m_Active = false;
	result->m_RefName = "Projectiles";
	result->m_type = _type;
	result->Init();
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}
