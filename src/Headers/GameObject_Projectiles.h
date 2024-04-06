/* Start Header ************************************************************************/
/*!
\file GameObject_Projectiles.h
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


#pragma once

#include "GameObject.h"
#include "Pch.h"

class GameObject_Projectiles : public GameObject
{
public:
	enum ProjectileType
	{
		P_FireBall_Red,
		P_FireBall_Blue,
		P_FireBall_Green,
		P_EnergyBall_Blue,
		P_EnergyBall_Dark,
		P_EnergyBall_Green,
		NUM_PROJECTILE_TYPES
	};

	GameObject_Projectiles(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_Projectiles();

	virtual void init();
	virtual void update(double _dt);
	virtual void render();
	virtual void exit();

	void fireAtPlayer(const AEVec2& _startpos = { -100.0f, -100.0f }, const AEVec2& _startscale = { 50.0f, 50.0f }, ProjectileType _type = ProjectileType::P_FireBall_Red, double _speed = 0.2f, double _MovTimer = 1.0f);

	ProjectileType m_type;
	double m_Speed;
private:
	//////////////////////////////////////////////////////////////////////////
	// Animation
	static constexpr u32 m_FIREBALL_SPRITESHEET_ROWS = 7;
	static constexpr u32 m_FIREBALL_SPRITESHEET_COLS = 9;
	static constexpr u32 m_FIREBALL_SPRITESHEED_MAX_SPRITES = 60;
	static constexpr f32 m_FIREBALL_SPRITE_UV_WIDTH = 1.f / m_FIREBALL_SPRITESHEET_COLS;
	static constexpr f32 m_FIREBALL_SPRITE_UV_HEIGHT = 1.f / m_FIREBALL_SPRITESHEET_ROWS;

	static constexpr u32 m_ENERGYBALL_SPRITESHEED_ROWS = 3;
	static constexpr u32 m_ENERGYBALL_SPRITESHEED_COLS = 5;
	static constexpr u32 m_ENERGYBALL_SPRITESHEED_MAX_SPRITES = 11;
	static constexpr f32 m_ENERGYBALL_SPRITE_UV_WIDTH = 1.f / m_ENERGYBALL_SPRITESHEED_COLS;
	static constexpr f32 m_ENERGYBALL_SPRITE_UV_HEIGHT = 1.f / m_ENERGYBALL_SPRITESHEED_ROWS;

	// (Not so good) hardcoded values about our animation
	f32 m_animationTimer = 0.f;
	f32 m_animationDurationPerFrame = 0.02f;
	u32 m_currentSpriteIndex = 0; // start from first sprite
	f32 m_currentSpriteUvOffsetX = 0.f;
	f32 m_currentSpriteUvOffsetY = 0.f;

	// Pointer to Mesh
	AEGfxVertexList* m_pFireBallMesh = 0;
	AEGfxVertexList* m_pEnergyBallMesh = 0;

};

namespace Create
{
	GameObject_Projectiles* projectiles( const AEVec2& _position = { 0.0f, 0.0f }, const AEVec2& _scale= { 200.0f, 200.0f}, GameObject_Projectiles::ProjectileType _type = GameObject_Projectiles::ProjectileType::P_FireBall_Red);
};
