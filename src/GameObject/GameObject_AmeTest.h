#pragma once
/////////////////////////////////////////////////////////////
// THIS IS AN EXAMPLE DELETE THIS FOR BETA TEST
// ///////////////////////////////////////////////////////////
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "../GameObject/GameObject.h"
#include "../Backend/2DCollider.h"

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
	void SetAABB(Vector3 maxAABB, Vector3 minAABB);
private:

};

namespace Create
{
	GameObject_AmeTest* Entity(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};
