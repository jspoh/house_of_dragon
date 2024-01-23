#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "../Backend/Vector3.h"

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_WALL,
		GO_FLIPPERWALL,
		GO_MOVINGWALL,
		GO_PILLAR,
		GO_BOUNDARY,
		GO_PLAYER,
		GO_MACEBALL,
		GO_POWERUP,
		GO_ENEMY1,
		GO_ENEMY2,
		GO_ENEMY3,
		GO_ENEMY_BULLET,
		GO_ENEMY_BOUNCINGBULLET,
		GO_BOX,
		GO_BOXPARTS,
		GO_ARROW,
	};

	enum POWERUP_TYPE //All PowerUp
	{
		ScatterShot,//Quantity Over Quality
		MagicMushroom,//All Stats Up, right?
		DarkMatter,//Caution: Do Not Touch
		ShotgunCoffee,//Gives you Wings
		CascadingCascada,//So irritating
		InfinityOrb,//It's over 9000
		HellRounds,//Paint the world in black
		HeartBackpack,//You have received a heart container
		CrystalHeart,//Become a diamond when hurt
		PaydayMask//Why so serious?
	};

	//
	GAMEOBJECT_TYPE type;

	Vector3 pos;
	Vector3 vel;
	Vector3 Momentum;
	Vector3 normal;
	Vector3 ShootingVel;
	Vector3 scale;
	float OriginalScale;
	Vector3 OriginalPosition;
	Vector3 dir;//direction.orientation
	Vector3 m_torque;
	float momentOfInertia;
	float angularVelocity; //in radians
	bool active;
	float mass;
	float angle;
	float bulletCooldown;
	int health;
	int MaximumHealth;//Special case for objects that require healing and other stuff
	float InvincibilityTime;
	GameObject* target;

	//FlipperWall
	float FlipperSpeed; //FlipperWall speed
	Vector3 FlipperCollidedVector;
	//SpringWall
	float WindingSpeed;

	//Powerup
	POWERUP_TYPE PowerUp;
	std::string PowerupName;
	std::string PowerupDesc;
	
	//Boss
	float BossDamage;
	float PhaseCooldown;

	//m_goList
	bool Collided;
	bool Stopped;
	float StopTimer;
	Vector3 CollidedVel;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
	void InsertPowerupDetails();
};

#endif