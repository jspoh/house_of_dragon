#pragma once
#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "../GameObject/GameObject.h"
#include "../Backend/2DCollider.h"

class GenericEntity : public GameObject, public CCollider
{
public:
	GenericEntity(/*Maybe put texture here still thinking*/);
	virtual ~GenericEntity();

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
	GenericEntity* Entity(const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));


};
#endif
