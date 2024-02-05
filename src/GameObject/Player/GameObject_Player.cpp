#include "../../GameObject/Player/GameObject_Player.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../Backend/MyMath.h"
//#include"../../Backend/Pch.h"
GameObject_Player::GameObject_Player()
{
}

GameObject_Player::~GameObject_Player()
{	
}

void GameObject_Player::Init()
{
	m_LocalPos.x = -600;
	m_LocalPos.y = -280;

	AEGfxMeshStart();
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
	pTex = AEGfxTextureLoad("Assets/walk.png");
	current_sprite_index = rand() % 8;

	

}

bool GameObject_Player::getIspplaying()
{
	return isPlaying;

}

void GameObject_Player::setIsplaying(bool check) 
{
	isPlaying = check;
}

void GameObject_Player::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
	// Update the animation timer.
	// animation_timer should go up to animation_duration_per_frame.
	animation_timer += (f32)AEFrameRateControllerGetFrameTime() * 2;
	timer += (f32)AEFrameRateControllerGetFrameTime();
	
	if (getIspplaying() == true)
	{
		movement_dash(this, Vector3(1000, 1000), 100, _dt);
	}

	if (AEInputCheckCurr(AEVK_A)) {
		this->m_LocalPos.x -= speed * _dt;
		if (animation_timer >= animation_duration_per_frame)
		{
			animation_timer = 0;

			// Calculate the next sprite UV
			current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

			u32 current_sprite_row = current_sprite_index / spritesheet_cols;
			u32 current_sprite_col = current_sprite_index % spritesheet_cols;

			current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
			current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;
		}
	}
	//correct direction 

	if (AEInputCheckTriggered(AEVK_D)) {
		//this->m_LocalPos.x += speed * _dt;
		//
		setIsplaying(true);
		if (animation_timer >= animation_duration_per_frame)
		{
			animation_timer = 0;

			// Calculate the next sprite UV
			current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

			u32 current_sprite_row = current_sprite_index / spritesheet_cols;
			u32 current_sprite_col = current_sprite_index % spritesheet_cols;

			current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
			current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;
		}
	}

	if (AEInputCheckCurr(AEVK_W)) {
		this->m_LocalPos.y += speed * _dt;

		if (animation_timer >= animation_duration_per_frame)
		{
			animation_timer = 0;
			
			// Calculate the next sprite UV
			current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

			u32 current_sprite_row = current_sprite_index / spritesheet_cols;
			u32 current_sprite_col = current_sprite_index % spritesheet_cols;

			current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
			current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;
		}
	}

	if (AEInputCheckCurr(AEVK_S)) {
		this->m_LocalPos.y -= speed * _dt;
		if (animation_timer >= animation_duration_per_frame)
		{
			animation_timer = 0;

			// Calculate the next sprite UV
			current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

			u32 current_sprite_row = current_sprite_index / spritesheet_cols;
			u32 current_sprite_col = current_sprite_index % spritesheet_cols;

			current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
			current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;
		}
	}



	//nearly equal

	//if (AEInputCheckTriggered(AEVK_A))
	//	this->m_LocalPos.x -= speed * _dt;
	//if (AEInputCheckTriggered(AEVK_D))
	//	this->m_LocalPos.x += speed * _dt;
	//if (AEInputCheckTriggered(AEVK_W))
	//	this->m_LocalPos.y += speed * _dt;
	//if (AEInputCheckTriggered(AEVK_S))
	//	this->m_LocalPos.y -= speed * _dt;



	//if (animation_timer >= animation_duration_per_frame) 
	//{
	//	// When the time is up go to the next sprite.

	//	// Reset the timer.
	//	animation_timer = 0;

	//	// Calculate the next sprite UV
	//	current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

	//	u32 current_sprite_row = current_sprite_index / spritesheet_cols;
	//	u32 current_sprite_col = current_sprite_index % spritesheet_cols;

	//	current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
	//	current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;

	//}

}

void GameObject_Player::Render()
{
	// Does nothing here, can inherit & override or create your own version of this class :D
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
	AEMtx33Rot(&rotate, 0);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &tran, &transform);

	//AEMtx33 scale = { 0 };
	//AEMtx33 transform = { 0 };
	//AEMtx33Scale(&scale, m_Scale.x, m_Scale.y);
	//AEMtx33Trans(&transform, m_LocalPos.x, m_LocalPos.y);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);



}

void GameObject_Player::Exit()
{
	AEGfxTextureUnload(pTex);
}

// Set the maxAABB and minAABB
void GameObject_Player::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GameObject_Player* Create::test(const std::string& _refName,
	const Vector3& _position,
	const Vector3& _scale)
{

	GameObject_Player* result = new GameObject_Player();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	result->m_RefName = _refName;
	result->Init();
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}


void movement_dash(GameObject_Player* player, Vector3 destination, float speed, double _dt)
{
	Vector3 angle = destination - player->m_LocalPos;

	float game_X = angle.x / sqrt(Math::Square(destination.x) + (destination.y));
	float game_Y = angle.y / sqrt(Math::Square(destination.x) + (destination.y));

	Vector3 direction_p(game_X, game_Y);


	
	if (player->timer <= 3.0f)
	{

		player->m_LocalPos += direction_p * speed * _dt;


	}

	if (player->timer >= 3.0f)
	{
		player->setIsplaying(false);
		player->timer = 0;
	}

}