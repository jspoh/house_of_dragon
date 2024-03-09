#pragma once

//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "GameObject.h"
#include "2DCollider.h"
#include "Pch.h"

class GameObject_Player : public GameObject, public CCollider
{
public:
	GameObject_Player(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_Player();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();
	bool getIspplaying();
	void setIsplaying(bool check);

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);
	f32 timer = 0;

private:

	float speed=5000.f;

	const u32 spritesheet_rows = 1;
	const u32 spritesheet_cols = 8;
	const u32 spritesheet_max_sprites = 16;
	const f32 sprite_uv_width = 1.f / spritesheet_cols;
	const f32 sprite_uv_height = 1.f / spritesheet_rows;

	// (Not so good) hardcoded values about our animation
	f32 animation_timer = 0.f;
	f32 animation_duration_per_frame = 0.1f;
	u32 current_sprite_index = 0; // start from first sprite
	f32 current_sprite_uv_offset_x = 0.f;
	f32 current_sprite_uv_offset_y = 0.f;
	// Pointer to Mesh
	AEGfxVertexList* pMesh = 0;
	AEGfxTexture* pTex = nullptr;
	bool isPlaying = false;

};

namespace Create
{
	GameObject_Player* test(const std::string& _RefName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));

};

void movement_dash(GameObject_Player* player, Vector3 destination, float speed, double _dt);
