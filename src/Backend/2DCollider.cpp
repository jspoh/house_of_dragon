#include "Pch.h"
#include "2DCollider.h"

CCollider::CCollider()
	: maxAABB(Vector3( 0, 0, 0))
	, minAABB(Vector3( 0, 0, 0))
{
}


CCollider::~CCollider()
{
}

// Set the maxAABB and minAABB
void CCollider::SetAABB(Vector3 t_maxAABB, Vector3 t_minAABB)
{
	this->maxAABB = t_maxAABB;
	this->minAABB = t_minAABB;
}
// Set the maxAABB
void CCollider::SetMaxAABB(Vector3 t_maxAABB)
{
	this->maxAABB = t_maxAABB;
}
// Get the maxAABB
Vector3 CCollider::GetMaxAABB(void)
{
	return maxAABB;
}
// Set the minAABB
void CCollider::SetMinAABB(Vector3 t_minAABB)
{
	this->minAABB = t_minAABB;
}
// Get the minAABB
Vector3 CCollider::GetMinAABB(void)
{
	return minAABB;
}
