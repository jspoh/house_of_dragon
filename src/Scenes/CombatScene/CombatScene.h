#pragma once

#include "../../Scenes/Scene.h"
#include "../../GameObject/Animals/Elements.hpp"

#include <vector>
//Testing SpriteAnimation
class SceneManager;
class CombatScene : public Scene
{
public:
	CombatScene();
	~CombatScene();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CombatScene(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static CombatScene* sInstance; // The pointer to the object that gets registered
};


/*defining these functions here to chiong prototype, no time to make nicenice rn*/

class Mob {
public:
	double health;
	double dmg;
	Element element;

	Mob(Element element, double health, double dmg);

	bool isDead();

	/**
	 * .
	 * 
	 * \param target
	 * \param attackEl
	 * \return amount of damage dealt
	 */
	virtual double attack(Mob& target);

};

class Enemy : Mob {
public:

	Enemy(Element element, double health, double dmg);
};

class Cat : Enemy {
public:
	Cat(Element element = Element::Water, double health=100, double dmg=10);
};

class Player : Mob {
public:
	Player(double health = 100, double dmg = 10, Element element = Element::NO_ELEMENT);

	/**
	 * .
	 * 
	 * \param target
	 * \param attackEl element player attacked with
	 * \return damage dealt
	 */
	double attack(Mob& target, Element attackEl);
};
