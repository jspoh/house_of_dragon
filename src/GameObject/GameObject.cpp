/* Start Header ************************************************************************/
/*!
\file GameManager.cpp
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


#include "Pch.h"
#include "GameObject.h"
GameObject::GameObject() 
	:m_Scale{ 1.0f, 1.0f },
	m_LocalPos{ 0.0f, 0.0f },
	m_WorldPos{ 0.0f, 0.0f },
	m_Lifetime(0.0),
	m_Active(true),
	m_RefName("NIL")
{
}

GameObject::~GameObject()
{
}


void GameObject::Init()
{

}
void GameObject::Update([[maybe_unused]] double dt)
{
}
void GameObject::Render()
{

}
void GameObject::Exit()
{

}