#pragma once
/////////////////////////////////////////////////////////////
// THIS IS AN EXAMPLE DELETE THIS FOR BETA TEST
// ///////////////////////////////////////////////////////////
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "GameObject.h"
#include "2DCollider.h"
#include "Pch.h"
class GameObject_Misc_Enemy : public GameObject, public CCollider
{
public:
	GameObject_Misc_Enemy(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_Misc_Enemy();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();

	// Set the maxAABB and minAABB
	void SetAABB(Vector3 t_maxAABB, Vector3 t_minAABB);

private:

};

namespace Create
{
	GameObject_Misc_Enemy* MiscEnemy( const AEVec2& _position = { 0.0f,0.0f }, const AEVec2& _scale = {200.0f, 200.0f});
};
