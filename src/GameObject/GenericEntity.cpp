#include "../GameObject/GenericEntity.h"
#include "../GameObject/GameObjectManager.h"

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
	//MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	//modelStack.PushMatrix();
	//modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Scale(scale.x, scale.y, scale.z);
	//RenderHelper::RenderMesh(modelMesh);
	//modelStack.PopMatrix();
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
