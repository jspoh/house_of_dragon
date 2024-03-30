#include "Pch.h"
#include "GameObject_Misc_Enemy.h"
#include "GameObjectManager.h"


GameObject_Misc_Enemy::GameObject_Misc_Enemy()
{
	m_Active = false;
}

GameObject_Misc_Enemy::~GameObject_Misc_Enemy()
{
}

void GameObject_Misc_Enemy::Init()
{
}

void GameObject_Misc_Enemy::Update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);

	//Update Enemy Logic or anything basic
	if (m_Active)
	{

		Point pos = wtos(m_LocalPos.x - camOffset.x, m_LocalPos.y - camOffset.y);
		//check Collision
		//std::cout << mouseX << ", " << mouseY << " | " << pos.x << ", " << pos.y << "\n";
		if (CollisionChecker::isMouseInRect(pos.x, pos.y, m_Scale.x, m_Scale.y, static_cast<float>(mouseX), static_cast<float>(mouseY)))
				std::cout << "Collision with misc enemy is working" << std::endl;
	}
}

void GameObject_Misc_Enemy::Render()
{
	if (m_Active)
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
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("MISC_ENEMY_STRONG"), 0, 0);

		AEMtx33 transform = { 0 };
		AEMtx33 scale;
		AEMtx33Scale(&scale, m_Scale.x, m_Scale.y);
		AEMtx33 trans;
		AEMtx33Trans(&trans, m_LocalPos.x, m_LocalPos.y);
		AEMtx33 rotate;
		AEMtx33Rot(&rotate, 0);

		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &trans, &transform);

		// Choose the transform to use
		AEGfxSetTransform(transform.m);
		// Actually drawing the mesh 
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	}
}

void GameObject_Misc_Enemy::Exit()
{
}

// Set the maxAABB and minAABB
void GameObject_Misc_Enemy::SetAABB(Vector3 t_maxAABB, Vector3 t_minAABB)
{
	this->maxAABB = t_maxAABB;
	this->minAABB = t_minAABB;
}

GameObject_Misc_Enemy* Create::MiscEnemy(const AEVec2& _position, const AEVec2& _scale)
{
	GameObject_Misc_Enemy* result = new GameObject_Misc_Enemy();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	result->m_Active = false;
	result->m_RefName = "MiscEnemy";
	result->Init();
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}
