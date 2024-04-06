/* Start Header ************************************************************************/
/*!
\file GenericEntity.h
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

#pragma once
#ifndef GENERIC_ENTITY_H
#define GENERIC_ENTITY_H
//Basic Entity in game (COPY THIS TO MAKE YOUR PLAYER ENTITY / ENEMY ENTITIES / ETC)
#include "GameObject.h"

class GenericEntity : public GameObject
{
public:
	GenericEntity(/*Maybe put texture here still thinking*/);
	virtual ~GenericEntity();

	virtual void init();
	virtual void update(double _dt);
	virtual void render();
	virtual void exit();
private:

};

namespace Create
{
	GenericEntity* entity(const std::string& _meshName,
		const AEVec2& _position,
		const AEVec2& _scale = { 1.0f, 1.0f });


};
#endif
