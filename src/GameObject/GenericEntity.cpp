#include "GenericEntity.h"
#include "GameObjectManager.h"
#include "Pch.h"
GenericEntity::GenericEntity()
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Init()
{

}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::Render()
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::Exit()
{
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	/*Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;*/

	GenericEntity* result = new GenericEntity();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}
