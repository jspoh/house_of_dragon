#include "../GameObject/GameObject_AmeTest.h"
#include "../GameObject/GameObjectManager.h"


GameObject_AmeTest::GameObject_AmeTest()
{
}

GameObject_AmeTest::~GameObject_AmeTest()
{
}

void GameObject_AmeTest::Init()
{
	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, sprite_uv_height, // bottom left
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top left

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
		0.5f, 0.5f, 0xFFFFFFFF, sprite_uv_width, 0.0f,   // top right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // bottom left


	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	// Load texture
	pTex = AEGfxTextureLoad("Assets/ame.png");

	current_sprite_index = rand() % 8;
}

void GameObject_AmeTest::Update(double _dt)
{
	// Update the animation timer.
	   // animation_timer should go up to animation_duration_per_frame.
	animation_timer += (f32)AEFrameRateControllerGetFrameTime()*2;

	if (animation_timer >= animation_duration_per_frame) {
		// When the time is up go to the next sprite.

		// Reset the timer.
		animation_timer = 0;

		// Calculate the next sprite UV
		current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

		u32 current_sprite_row = current_sprite_index / spritesheet_cols;
		u32 current_sprite_col = current_sprite_index % spritesheet_cols;

		current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
		current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;

	}
}

void GameObject_AmeTest::Render()
{
	// Your own rendering logic goes here
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Set the texture to pTex
	AEGfxTextureSet(pTex, current_sprite_uv_offset_x, current_sprite_uv_offset_y);

	AEMtx33 transform = { 0 };
	AEMtx33 scale;
	AEMtx33Scale(&scale, m_Scale.x, m_Scale.y);

	AEMtx33 tran;
	AEMtx33Trans(&tran, m_LocalPos.x, m_LocalPos.y);

	AEMtx33 rotate;
	AEMtx33Rot(&rotate, PI / 4);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &tran, &transform);

	//AEMtx33 scale = { 0 };
	//AEMtx33 transform = { 0 };
	//AEMtx33Scale(&scale, m_Scale.x, m_Scale.y);
	//AEMtx33Trans(&transform, m_LocalPos.x, m_LocalPos.y);
	// Choose the transform to use
	AEGfxSetTransform(transform.m);
	// Actually drawing the mesh 
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void GameObject_AmeTest::Exit()
{
	AEGfxTextureUnload(pTex);
}

// Set the maxAABB and minAABB
void GameObject_AmeTest::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GameObject_AmeTest* Create::Ame(const std::string& _refName,
	const Vector3& _position,
	const Vector3& _scale)
{
	GameObject_AmeTest* result = new GameObject_AmeTest();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	result->m_RefName = _refName;
	result->Init();
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}
