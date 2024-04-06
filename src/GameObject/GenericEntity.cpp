/* Start Header ************************************************************************/
/*!
\file GenericEntity.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief base entity class
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "GenericEntity.h"
#include "GameObjectManager.h"
GenericEntity::GenericEntity()
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::init()
{

}

void GenericEntity::update(double _dt)
{
	UNREFERENCED_PARAMETER(_dt);
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::render()
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::exit()
{
}

GenericEntity* Create::entity(const std::string& _meshName,
	const AEVec2& _position,
	const AEVec2& _scale)
{
	/*Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;*/
	UNREFERENCED_PARAMETER(_meshName);
	GenericEntity* result = new GenericEntity();
	result->m_LocalPos = _position;
	result->m_Scale = _scale;
	//result->bool m_bCollider(false);
	GameObjectManager::getInstance()->addEntity(result);
	return result;
}
