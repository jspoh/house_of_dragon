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
	enum v_EnemyBehaviour
	{
		Normal,
		Hunt,
		Aggressive,
		Shy
	};

	GameObject_Misc_Enemy(/*Maybe put texture here still thinking*/);
	virtual ~GameObject_Misc_Enemy();

	virtual void Init();
	virtual void Update(double _dt);
	virtual void Render();
	virtual void Exit();

	void ActivateEnemy(AEMtx33& m_transform, v_EnemyBehaviour _type = Normal);
	void UpdateEnemyMov(double _dt);
	void UpdateCombatTransition(double _dt);

	int m_StartCombat; //0 - dont, 1 - Natural, 2 - Abrupt
private:
	AEMtx33* m_AttachedFloorTransform;
	AEMtx33 m_TransformData;
	bool m_AttackPlayer; //Trigger combat scene
	double m_LifeTime;
	int m_Health;
	f32 m_Transparency;
	double m_FlickeringTimer;
};

namespace Create
{
	GameObject_Misc_Enemy* MiscEnemy( const AEVec2& _position = { 0.0f,0.0f }, const AEVec2& _scale = {1.0f, 1.0f});
};
