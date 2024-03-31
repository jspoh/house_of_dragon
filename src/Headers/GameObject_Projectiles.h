#pragma once
/////////////////////////////////////////////////////////////
// THIS IS AN EXAMPLE DELETE THIS FOR BETA TEST
// ///////////////////////////////////////////////////////////
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "GameObject.h"
#include "2DCollider.h"
#include "Pch.h"

#define PLAYERSCREENPOSX 0
#define PLAYERSCREENPOSY 100

class GameObject_Projectiles : public GameObject, public CCollider
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
	};

	GameObject_Projectiles(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_Projectiles();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();

	void FireAtPlayer(const AEVec2& _startpos = { 0.0f, 0.0f }, const AEVec2& _startscale = { 0.0f, 0.0f }, ProjectileType _type = ProjectileType::P_FireBall_Red, double _speed = 0.2f, double _MovTimer = 1.0f);

	ProjectileType m_type;
	double m_Speed, m_LifeTime;
private:
	//////////////////////////////////////////////////////////////////////////
	// Animation
	const u32 FireBall_spritesheet_rows = 7;
	const u32 FireBall_spritesheet_cols = 9;
	const u32 FireBall_spritesheet_max_sprites = 60;
	const f32 FireBall_sprite_uv_width = 1.f / FireBall_spritesheet_cols;
	const f32 FireBall_sprite_uv_height = 1.f / FireBall_spritesheet_rows;

	const u32 EnergyBall_spritesheet_rows = 3;
	const u32 EnergyBall_spritesheet_cols = 5;
	const u32 EnergyBall_spritesheet_max_sprites = 11;
	const f32 EnergyBall_sprite_uv_width = 1.f / EnergyBall_spritesheet_cols;
	const f32 EnergyBall_sprite_uv_height = 1.f / EnergyBall_spritesheet_rows;

	// (Not so good) hardcoded values about our animation
	f32 animation_timer = 0.f;
	f32 animation_duration_per_frame = 0.02f;
	u32 current_sprite_index = 0; // start from first sprite
	f32 current_sprite_uv_offset_x = 0.f;
	f32 current_sprite_uv_offset_y = 0.f;

	// Pointer to Mesh
	AEGfxVertexList* pFireBallMesh = 0;
	AEGfxVertexList* pEnergyBallMesh = 0;

};

namespace Create
{
	GameObject_Projectiles* Projectiles( const AEVec2& _position = { 0.0f, 0.0f }, const AEVec2& _scale= { 200.0f, 200.0f}, GameObject_Projectiles::ProjectileType _type = GameObject_Projectiles::ProjectileType::P_FireBall_Red);
};
