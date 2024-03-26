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