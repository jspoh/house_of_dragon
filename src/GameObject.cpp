
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	Momentum(0.f,0.f,0.f),
	active(false),
	mass(1.f),
	angularVelocity(0.0f),
	angle(0.0f),
	momentOfInertia(0.0f),
	BossDamage(0.0f),
	target(nullptr),
	InvincibilityTime(0.0f),
	StopTimer(0.f)
{
}

GameObject::~GameObject()
{
}

void GameObject::InsertPowerupDetails()
{
	switch (PowerUp)
	{
	case ScatterShot:
		scale.Set(3.5, 5, 1);
		PowerupName = "ScatterShot";
		PowerupDesc = "Quantity over Quality";
		break;
	case MagicMushroom:
		scale.Set(5, 5, 1);
		PowerupName = "Magic Mushroom";
		PowerupDesc = "All Stats Up, right?";
		break;
	case DarkMatter:
		scale.Set(4, 5, 1);
		PowerupName = "Dark Matter";
		PowerupDesc = "Caution: Do Not Touch";
		break;
	case ShotgunCoffee:
		scale.Set(5, 5, 1);
		PowerupName = "Shotgun Coffee";
		PowerupDesc = "Gives you Wings";
		break;
	case CascadingCascada:
		scale.Set(7, 5, 1);
		PowerupName = "Cascading Cascade";
		PowerupDesc = "So irritating";
		break;
	case InfinityOrb:
		scale.Set(5, 5, 1);
		PowerupName = "Infinity Orb";
		PowerupDesc = "It's over 9000";
		break;
	case HellRounds:
		scale.Set(3, 5, 1);
		PowerupName = "Hell Rounds";
		PowerupDesc = "Paint the world in black";
		break;
	case HeartBackpack:
		scale.Set(5, 5, 1);
		PowerupName = "Heart Backpack";
		PowerupDesc = "You've got a heart container!";
		break;
	case CrystalHeart:
		scale.Set(4, 5, 1);
		PowerupName = "Crystal Heart";
		PowerupDesc = "Become a diamond when hurt";
		break;
	case PaydayMask:
		scale.Set(5, 5, 1);
		PowerupName = "Payday Mask";
		PowerupDesc = "Why so serious?";
		break;
	default:
		PowerupName = "Default";
		PowerupDesc = "Default";
		break;
	}
}