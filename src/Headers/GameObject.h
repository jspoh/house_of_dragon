/* Start Header ************************************************************************/
/*!
\file GameManager.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief GameObject base class
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

//BASE CLASS GAMEOBJECTS - CONSISTING OF COMMON FUNCTIONS AND VARIABLES
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	AEVec2 m_LocalPos, m_WorldPos; //World Pos for Game Logic Calculation, Local Pos for Graphic Calculation
	AEVec2 m_Scale;
	double m_Lifetime;
	bool m_Active; //True if alive, false if dead
	std::string m_RefName;
protected:

};
#endif