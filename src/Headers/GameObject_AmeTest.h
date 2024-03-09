#pragma once
/////////////////////////////////////////////////////////////
// THIS IS AN EXAMPLE DELETE THIS FOR BETA TEST
// ///////////////////////////////////////////////////////////
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "GameObject.h"
#include "2DCollider.h"
#include "Pch.h"
class GameObject_AmeTest : public GameObject, public CCollider
{
public:
	GameObject_AmeTest(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_AmeTest();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 t_maxAABB, Vector3 t_minAABB);

private:
	const u32 spritesheet_rows = 5;
	const u32 spritesheet_cols = 4;
	const u32 spritesheet_max_sprites = 19;
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
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/ame.png");
};

namespace Create
{
	GameObject_AmeTest* Ame(const std::string& _RefName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
