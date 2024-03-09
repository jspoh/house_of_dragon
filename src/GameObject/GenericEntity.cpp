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
	UNREFERENCED_PARAMETER(_dt);
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
void GenericEntity::SetAABB(Vector3 t_maxAABB, Vector3 t_minAABB)
{
	this->maxAABB = t_maxAABB;
	this->minAABB = t_minAABB;
}

GenericEntity* Create::Entity(const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale)
{
	/*Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;*/
	UNREFERENCED_PARAMETER(_meshName);
	GenericEntity* result = new GenericEntity();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	//result->bool m_bCollider(false);
	GameObjectManager::GetInstance()->AddEntity(result);
	return result;
}
