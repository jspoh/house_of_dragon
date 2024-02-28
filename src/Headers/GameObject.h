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

	Vector3 m_LocalPos, m_WorldPos; //World Pos for Game Logic Calculation, Local Pos for Graphic Calculation
	Vector3 m_Scale;
	double m_Lifetime;
	bool m_Active; //True if alive, false if dead
	std::string m_RefName;
protected:

};
#endif